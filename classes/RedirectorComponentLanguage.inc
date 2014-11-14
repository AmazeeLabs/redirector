<?php

/**
 * @file
 *  Redirect component based on language.
 */

class RedirectorComponentLanguage extends RedirectorComponent {

  /**
   * Constructor of the class. We initialize the types.
   */
  public function __construct() {
    $this->types = array('must_match_restriction');
  }

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
   * @see RedirectorComponentInterface::getStorageColumns()
   */
	public function getStorageColumns() {
    return array('language');
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getAvailableOptions()
   */
  public function getAvailableOptions($column_name = NULL) {
    $languages = language_list();
    $options['language'] = array();
    foreach ($languages as $language_object) {
      $options['language'][$language_object->language] = $language_object->name;
    }
    if (!empty($column_name)) {
      return $options[$column_name];
    }
    return $options;
  }
}
