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
      $column_matches = FALSE;
      if (is_string($value[$column_name]['value'])) {
        if ($value[$column_name]['value'] === '--any--' || $value[$column_name]['value'] == $current_value[$column_name]) {
          $column_matches = TRUE;
        }
      }
      elseif (is_array($value[$column_name]['value'])) {
        foreach ($value[$column_name]['value'] as $value_key => $value_value) {
          if ($value_value === '--any--' || $value_value == $current_value[$column_name]) {
            $column_matches = TRUE;
          }
        }
      }
      // Check if we should negate the result.
      if (!empty($value[$column_name]['negate'])) {
        $column_matches = !$column_matches;
      }

      // If we find at least one column that does not match, then we break here.
      if (!$column_matches) {
        $match = FALSE;
        break;
      }
    }
    return $match;
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::summary()
   */
  public function summary($values) {
    static $options;
    if (!isset($options)) {
      $options = $this->getAvailableOptions();
    }
    $output = $this->getTitle() . ': ';
    foreach ($values as $key => $value) {
      $readable_values = array();
      if (!empty($value['value'])) {
        if (is_array($value['value'])) {
          foreach ($value['value'] as $option_value) {
            if (!empty($options[$key][$option_value])) {
              $readable_values[$key][] = $options[$key][$option_value];
            }
            else {
              $readable_values[$key][] = $option_value;
            }
          }
        }
        else {
          if (!empty($options[$key][$value['value']])) {
            $readable_values[$key] = $options[$key][$value['value']];
          }
          else {
            $readable_values[$key] = $value['value'];
          }
        }
      }
      if (!empty($value['negate'])) {
        $output .= 'NOT ';
      }
      if (is_array($readable_values[$key])) {
        $output .= ' IN (' . implode('; ', $readable_values[$key]) . '); ';
      }
      else {
        $output .= $readable_values[$key] . '; ';
      }
    }
    return $output;
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
