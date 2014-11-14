<?php

/**
 * @file
 *  Abstract class for all Redirector Components classes.
 */

abstract class RedirectorComponent implements RedirectorComponentInterface {

  /**
   * An array with types of this component.
   */
  protected $types = array();

  /**
   * (non-PHPdoc)
   * @see RedirectorComponentInterface::isOfType()
   */
  public function isOfType($type) {
    return in_array($type, $this->types);
  }
}
