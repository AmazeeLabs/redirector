<?php

/**
 * @file
 *  Redirector class based on country.
 */

class RedirectorComponentCountry extends RedirectorComponent {

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getTitle()
   */
  public function getTitle() {
    // @todo: Should we translate it here?
    return 'Country';
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getStorageColumns()
   */
	public function getStorageColumns() {
    return array('country');
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getAvailableOptions()
   */
  public function getAvailableOptions($column_name = NULL) {
    $options['country'] = country_get_list();
    if (!empty($column_name)) {
      return $options[$column_name];
    }
    return $options;
  }
}
