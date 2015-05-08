<?php

/**
 * @file
 *  Redirect component based on language.
 */

class RedirectorComponentLanguage extends RedirectorComponent {

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getTitle()
   */
  public function getTitle() {
    // @todo: Should we translate it here?
    return 'Language';
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getAvailableOptions()
   */
  public function getAvailableOptions() {
    $languages = language_list();
    $options = array();
    foreach ($languages as $language_object) {
      $options[$language_object->language] = $language_object->name;
    }
    return $options;
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getCurrentValue()
   */
  public function getCurrentValue() {
    global $language;
    return $language->language;
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::prepareRedirect()
   */
  public function prepareRedirect($values, &$url, &$options) {
    $language_list = language_list();
    $lang_code = NULL;
    if (is_string($values['value'])) {
      $lang_code= $values['value'];
    }
    elseif (is_array($values['value'])) {
      $lang_code = reset($values['value']);
    }

    // Alter the langauge of the options array.
    if (!empty($lang_code) && $lang_code != '--any--' && !empty($language_list[$lang_code])) {
      $options['language'] = $language_list[$lang_code];
    }
  }
}
