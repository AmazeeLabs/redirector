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
   *  columns that the component defines. Each element in the array is another
   *  array with two keys:
   *   - value: it can be a string representing the value of the component or
   *   an array representing the multiple values of the component.
   *   - negate: if we should negate the result.
   */
  public function matchValue(array $value);

  /**
   * Prepares the redirect using $values.
   *
   * @param array $values
   *  An array with the values of the component, like in
   *  RedirectorComponentInterface::matchValue().
   *
   * @param string $url
   *  The URL to which the redirect will be made.
   *
   * @param array $options
   *  An array with options for the URL. This is the same as the $options
   *  parameter for the url() function.
   */
  public function prepareRedirect($values, &$url, &$options);

  /**
   * Returns a string representing the summary of a component for a set of
   * values.
   *
   * @param array $values
   *  An array with values having the same structure as the one in
   *  RedirectorComponentInterface::matchValue().
   */
  public function summary($values);

}
