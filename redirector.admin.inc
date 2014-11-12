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
    $options[$key] = $component['title'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function redirector_admin_rules_page() {
  return array('#markup' => 'rules');
}
