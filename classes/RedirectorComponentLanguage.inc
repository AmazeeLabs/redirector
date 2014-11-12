<?php

/**
 * @file
 *  Redirect component based on language.
 */

class RedirectorComponentLanguage implements RedirectorComponentInterface {

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getStorageColumns()
   */
	public function getStorageColumns() {
    return array('language');
  }
}