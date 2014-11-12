<?php

/**
 * @file
 *  Interface for redirector component classes.
 */

interface RedirectorComponentInterface {
  /**
   * Returns an array with the name of the columns used to store the data for
   * this component in the storage (database).
   */
  public function getStorageColumns();

}
