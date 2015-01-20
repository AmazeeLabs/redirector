<?php

/**
 * @file
 *  Abstract class for all Redirector Components classes.
 */

abstract class RedirectorComponent implements RedirectorComponentInterface {

  /**
   * The weight of the component.
   */
  protected $weight = 0;

  /**
   * Constructor for a component.
   *
   * @param array $params
   *  An array with parameters to initialize the component. It can contain the
   *  following values:
   *   - 'weight': the weight of the component.
   */
  public function __construct($params = array()) {
    if (!empty($params['weight'])) {
      $this->weight = $params['weight'];
    }
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::matchValues()
   */
  public function matchValue(array $value) {
    // The default implementation is to check every field.
    $columns = $this->getStorageColumns();
    $match = TRUE;
    $current_value = $this->getCurrentValue();
    foreach ($columns as $column_name) {
      // If a column name is missing, then we do not match it.
      if (!isset($value[$column_name])) {
        return FALSE;
      }
      // If any value should match for this column, then just continue.
      if ($value[$column_name] === '--any--') {
        continue;
      }
      // If we find at least one column that do not match, then we break here.
      if ($value[$column_name] != $current_value[$column_name]) {
        $match = FALSE;
        break;
      }
    }
    return $match;
  }

  /**
   * Implementation of the __get() overload method.
   */
  public function __get($name) {
    if (isset($this->{$name})) {
      return $this->{$name};
    }
    return NULL;
  }
}
