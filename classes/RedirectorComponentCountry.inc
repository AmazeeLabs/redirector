<?php

/**
 * @file
 *  Redirector class based on country.
 */

class RedirectorComponentCountry implements RedirectorComponentInterface {

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getStorageColumns()
   */
	public function getStorageColumns() {
    return array('country');
  }

}
