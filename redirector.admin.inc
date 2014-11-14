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
  foreach ($components as $key => $component_name) {
    $component_object = redirector_get_component($key);
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
    'current_rules' => array('#markup' => 'current rules'),
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
    foreach ($active_components as $key => $component_name) {
      $component = redirector_get_component($key);
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
    $message = t('The rule has been updated');
  }
  else {
    $redirect_rule = entity_get_controller('redirector_rule')->create();
  }

  // Add the new values into the object.
  $redirect_rule->title = $form_state['values']['title'];
  // @todo: SAVE THE COMPONENTS!

  // And save it.
  entity_get_controller('redirector_rule')->save($redirect_rule);
  drupal_set_message($message);
  $form_state['redirect'] = 'admin/config/system/redirector/rules';
}
