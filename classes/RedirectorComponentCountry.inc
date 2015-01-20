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

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getCurrentValue()
   */
  public function getCurrentValue() {
    $country = '';
    // If we have the country in the request parameters, then we use it.
    if (!empty($_REQUEST['redirector_override_country_code'])) {
      $country = $_REQUEST['redirector_override_country_code'];
    }
    else {
      $geoip = array();
      if (function_exists('geoip_record_by_name')) {
        $geoip = @geoip_record_by_name(ip_address());
      }
      if (isset($geoip['country_code'])) {
        $country = drupal_strtoupper($geoip['country_code']);
      }
    }

    return array('country' => $country);
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::prepareRedirect()
   */
  public function prepareRedirect($values, &$url, &$options) {
    $options['query']['redirector_override_country_code'] = $values['country'];
  }
}
