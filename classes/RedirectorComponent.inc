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
   * The id of this component.
   */
  protected $id;

  /**
   * Constructor for a component.
   *
   * @param array $params
   *  An array with parameters to initialize the component. It can contain the
   *  following values:
   *   - 'weight': the weight of the component.
   */
  public function __construct($id, $params = array()) {
    $this->id = $id;
    if (!empty($params['weight'])) {
      $this->weight = $params['weight'];
    }
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::getId()
   */
  public function getId() {
    return $this->id;
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::matchValues()
   */
  public function matchValue(array $value) {
    $current_value = $this->getCurrentValue();
    $matches = FALSE;
    if (is_string($value['value'])) {
      if ($value['value'] === '--any--' || $value['value'] == $current_value) {
        $matches = TRUE;
      }
    }
    elseif (is_array($value['value'])) {
      foreach ($value['value'] as $value_key => $value_value) {
        if ($value_value === '--any--' || $value_value == $current_value) {
          $matches = TRUE;
        }
      }
    }
    // Check if we should negate the result.
    if (!empty($value['negate'])) {
      $matches = !$matches;
    }
    return $matches;
  }

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::summary()
   */
  public function summary($value) {
    static $options;
    if (!isset($options)) {
      $options = $this->getAvailableOptions();
    }
    $output = $this->getTitle() . ': ';
    $readable_values = array();
    if (!empty($value['value'])) {
      if (is_array($value['value'])) {
        foreach ($value['value'] as $option_value) {
          if (!empty($options[$option_value])) {
            $readable_values[] = $options[$option_value];
          }
          else {
            $readable_values[] = $option_value;
          }
        }
      }
      else {
        if (!empty($options[$value['value']])) {
          $readable_values = $options[$value['value']];
        }
        else {
          $readable_values = $value['value'];
        }
      }
    }
    if (!empty($value['negate'])) {
      $output .= 'NOT ';
    }
    if (is_array($readable_values)) {
      $output .= ' IN (' . implode('; ', $readable_values) . '); ';
    }
    else {
      $output .= $readable_values . '; ';
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
