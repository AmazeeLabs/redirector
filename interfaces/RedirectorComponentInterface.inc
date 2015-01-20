<?php

/**
 * @file
 *  Interface for redirector component classes.
 */

interface RedirectorComponentInterface {

  /**
   * Returns the title of the component.
   */
  public function getTitle();

  /**
   * Returns an array with the name of the columns used to store the data for
   * this component in the storage (database).
   */
  public function getStorageColumns();

  /**
   * Returns an array with all the options that this component can have.
   *
   * This can be used for example on an admin form to show all the values that
   * this component can have inside a redirector rule.
   */
  public function getAvailableOptions($column_name = NULL);

  /**
   * Returns the value of the component, based on the current request.
   */
  public function getCurrentValue();

  /**
   * Checks if the value of the component matches the value sent as parameter.
   *
   * @param array $value
   *  An array with values to match. This usually contains as keys all the
   *  columns that the component defines.
   */
  public function matchValue(array $value);

}
