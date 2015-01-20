<?php

/**
 * @file
 *  Page callbacks for the administration sites.
 */

/**
 * Menu callback for the components admin form.
 */
function redirector_admin_components_form() {
  $all_components = redirector_get_all_components();
  $options = array();
  foreach ($all_components as $key => $component) {
    $component_object = redirector_get_component($key);
    if (!empty($component_object)) {
      $options[$key] = $component_object->getTitle();
    }
  }

  $current_components = redirector_get_active_components();
  $default_value = array_keys($current_components);
  $form['redirector_components'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Components'),
    '#options' => $options,
    '#default_value' => $default_value,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for the components admin form.
 */
function redirector_admin_components_form_submit(&$form, &$form_state) {
  $components = array_filter($form_state['values']['redirector_components']);
  variable_set('redirector_components', $components);
  redirector_update_table_fields();
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Helper function to update the table fields based on the current active
 * components.
 */
function redirector_update_table_fields() {
// For each active components, make sure we have the fields. If not, we add
  // them now.
  $components = redirector_get_active_components();
  $all_components = redirector_get_all_components();
  // Rebuild the schema, to make sure we have the newest fields.
  $new_schema = drupal_get_schema('redirector_rules', TRUE);
  foreach ($components as $key => $component_object) {
    if (!empty($component_object)) {
      $columns = $component_object->getStorageColumns();
      // We iterate over each column and check if we already have an column for
      // it. If not, then we create one now.
      foreach ($columns as $column_name) {
        if (!db_field_exists('redirector_rules', $column_name)) {
          db_add_field('redirector_rules', $column_name, $new_schema['fields'][$column_name]);
        }
      }
    }
  }
}

/**
 * Menu callback for the admin rules page.
 */
function redirector_admin_rules_page() {
  return array(
    'current_rules' => drupal_get_form('redirector_current_rules_form'),
    'add_rule' => drupal_get_form('redirector_admin_add_edit_rule_form'),
  );
}

/**
 * Form constructor for adding/editing a rule.
 */
function redirector_admin_add_edit_rule_form($form, &$form_state, $rule = NULL) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($rule) ? $rule->title : '',
    '#required' => TRUE,
  );
  $active_components = redirector_get_active_components();
  if (!empty($active_components)) {
    $form['components'] = array(
      '#type' => 'fieldset',
      '#title' => t('Components'),
    );
    // @todo: It is possible that we will need to refactor or to extend this.
    // The way it works now is that if the component returns some options for
    // a column, then we will show a simple select item, otherwise we will show
    // a textfield.
    $component_options = array();
    foreach ($active_components as $key => $component) {
      // The component options are used below, in the must_match field.
      $component_options[$key] = $component->getTitle();
      $columns = $component->getStorageColumns();
      foreach ($columns as $column_name) {
        $options = $component->getAvailableOptions($column_name);
        if (!empty($options)) {
          $options = array_reverse($options, TRUE);
          $options['--any--'] = t('Any');
          $options = array_reverse($options, TRUE);
        }
        $form['components'][$column_name] = array(
          '#type' => !empty($options) ? 'select' : 'textfield',
          '#title' => $component->getTitle() . ' - (' . $column_name . ')',
          '#default_value' => !empty($rule) ? $rule->{$column_name} : '',
        );
        if (!empty($options)) {
          $form['components'][$column_name]['#options'] = $options;
        }
      }
    }
    $form['components']['must_match'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Components that must match the rule'),
      '#description' => t('Select the components that must match when checking a rule for redirect. The ones which may not match are still used to find the best redirect URL when there are several possibilities.'),
      '#options' => $component_options,
      '#default_value' => !empty($rule->must_match) ? $rule->must_match : array(),
    );
  }
  if (!empty($rule)) {
    $form['rid'] = array(
      '#type' => 'value',
      '#value' => $rule->rid,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Submit handler for the add/edit rule form.
 */
function redirector_admin_add_edit_rule_form_submit(&$form, &$form_state) {
  // Load the redirector test if we have one, or create a new one otherwise.
  $message = t('The rule has been saved.');
  if (!empty($form_state['values']['rid'])) {
    $redirect_rule = redirector_rule_load($form_state['values']['rid']);
    $message = t('The rule has been updated.');
  }
  else {
    $redirect_rule = entity_get_controller('redirector_rule')->create();
  }

  // Add the new values into the object.
  $redirect_rule->title = $form_state['values']['title'];
  $redirect_rule->must_match = array_filter($form_state['values']['must_match']);
  $active_components = redirector_get_active_components();
  foreach ($active_components as $key => $component) {
    $columns = $component->getStorageColumns();
    foreach ($columns as $column_name) {
      $redirect_rule->{$column_name} = $form_state['values'][$column_name];
    }
  }

  // And save it.
  entity_get_controller('redirector_rule')->save($redirect_rule);
  drupal_set_message($message);
  $form_state['redirect'] = redirector_get_admin_path() . '/rules';
}

/**
 * Form constructor for showing the current rules form.
 */
function redirector_current_rules_form($form, &$form_state) {
  $active_components = redirector_get_active_components();
  $header = array('title' => t('Title'));
  foreach ($active_components as $component) {
    $columns = $component->getStorageColumns();
    foreach ($columns as $column_name) {
      $header[$column_name] = $component->getTitle() . ' - (' . $column_name . ')';
    }
  }
  $header['actions'] = t('Actions');

  $component_rules = redirector_rule_load_multiple();
  $rows = array();
  $weight_delta = round(count($component_rules) / 2) + 10;
  $yes = '<span style="font-weight:bold; text-transform:uppercase">' . t('yes') . '</span>';
  $no = t('no');
  foreach ($component_rules as $rule) {
    $row = array();
    foreach ($header as $key => $value) {
      if ($key == 'actions') {
        $actions = array();
        $actions[] = l(t('Edit'), redirector_edit_rule_link($rule->rid));
        $actions[] = l(t('Delete'), redirector_delete_rule_link($rule->rid));
        $row[] = implode(' | ', $actions);
      }
      else {
        $must_match = $no;
        if (!empty($rule->must_match[$key])) {
          $must_match = $yes;
        }
        $row[] = $rule->{$key} . ' (' . t('Must match: !must_match', array('!must_match' => $must_match)) . ')';
      }
    }
    $rows[$rule->rid] = array('data' => $row, 'class' => array('draggable'));
    $form['weight_' . $rule->rid] = array(
      '#type' => 'weight',
      '#default_value' => $rule->weight,
      '#delta' => $weight_delta,
      '#attributes' => array('class' => array('component-rules-weight')),
      '#title' => t('Weight'),
      '#title_display' => 'invisible',
    );
    $rids[] = $rule->rid;
  }
  $form['rids'] = array(
    '#type' => 'value',
    '#value' => $rids,
  );
  $form['#header'] = $header;
  $form['#rows'] = $rows;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler fro the current rules form.
 *
 * This basically just updates the weight of the rules.
 */
function redirector_current_rules_form_submit(&$form, &$form_state) {
  foreach ($form_state['values']['rids'] as $rid) {
    $rule = redirector_rule_load($rid);
    $rule->weight = $form_state['values']['weight_' . $rid];
    entity_get_controller('redirector_rule')->save($rule);
  }
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Theme for the redirector_current_rules_form form.
 */
function theme_redirector_current_rules_form(&$variables) {
  $form = $variables['form'];
  $table_id = 'current-rules-table';
  $form['#header']['weight'] = t('Weight');
  foreach ($form['#rows'] as $rid => &$value) {
    $form['#rows'][$rid]['data']['weight'] = drupal_render($form['weight_' . $rid]);
  }
  $output = theme('table', array('header' => $form['#header'], 'rows' => $form['#rows'], 'attributes' => array('id' => $table_id)));
  $output .= drupal_render_children($form);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'component-rules-weight');
  return $output;
}

/**
 * Confirm for for deleting a rule.
 */
function redirector_admin_delete_rule_confirm($form, &$form_state, $rule) {
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $rule->rid,
  );
  $message = t('Are you sure you want to delete the %title rule? This action cannot be undone', array('%title' => $rule->title));
  return confirm_form($form, t('Are you sure you want to delete the rule?'), redirector_get_admin_path() . '/rules', $message);
}

/**
 * Submit handler for the rule deletion form.
 */
function redirector_admin_delete_rule_confirm_submit(&$form, &$form_state) {
  entity_delete('redirector_rule', $form_state['values']['rid']);
  drupal_set_message(t('The rule has been deleted.'));
  $form_state['redirect'] = redirector_get_admin_path() . '/rules';
}
