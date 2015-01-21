<?php

/**
 * @file
 *  Controller class for the RedirectorTest Entity.
 */

class RedirectorRuleEntityController extends DrupalDefaultEntityController implements EntityAPIControllerInterface {

  /**
   * (non-PHPdoc)
   * @see DrupalDefaultEntityController::attachLoad()
   */
  public function attachLoad(&$entities, $revision_id = false) {
    // not used at the moment.
    $parts = redirector_rule_parts();
    $active_components = redirector_get_active_components();
    // @todo: we need to somehow improve this. The arrays should always be small
    // but we should either cache the entire entities, or improve this.
    // Caching would be pretty ok actually.
    foreach ($entities as $entity) {
      foreach ($parts as $part => $part_value) {
        foreach ($active_components as $component_key => $component) {
          $columns = $component->getStorageColumns();
          foreach ($columns as $column) {
            if (!empty($entity->{$part . '_' . $column})) {
              $entity->{$part . '_' . $column} = unserialize($entity->{$part . '_' . $column});
            }
          }
        }
      }
    }
  }


  public function delete($ids) {
    db_delete($this->entityInfo['base table'])->condition('rid', $ids)->execute();
  }

  // The next methods are not yet supported. We have to implement them.
/* (non-PHPdoc)
 * @see EntityAPIControllerInterface::invoke()
 */public function invoke($hook, $entity) {
    // TODO Auto-generated method stub
    }

  /**
   * (non-PHPdoc)
   * @see EntityAPIControllerInterface::save()
   */
  public function save($entity) {
    if (empty($entity->{$this->idKey}) || !empty($entity->is_new)) {
      $op = 'insert';
      $return = drupal_write_record($this->entityInfo['base table'], $entity);
    }
    else {
      $op = 'update';
      $return = drupal_write_record($this->entityInfo['base table'], $entity, $this->idKey);
    }
    // @todo: this is not wokring at the moment!
    //$this->invoke($op, $entity);
  }

  /**
   * (non-PHPdoc)
   * @see EntityAPIControllerInterface::create()
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE, 'created' => REQUEST_TIME);

    // If there is a class for this entity type, instantiate it now.
    if (isset($this->entityInfo['entity class']) && $class = $this->entityInfo['entity class']) {
      $entity = new $class($values, $this->entityType);
    }
    else {
      // Otherwise use a good old stdClass.
      $entity = (object) $values;
    }

    // Allow other modules to alter the created entity.
    // We will enable this if really needed.
    //drupal_alter('redirector_test_entity_create', $this->entityType, $entity);

    return $entity;
  }

  /**
   * Checks if this rule matches a component.
   *
   * @param stdClass $entity
   *   The entity to check, which contains the rule definition.
   *
   * @param RedirectorComponentInterface $component
   *   The component to be checked.
   *
   * @param string $match_parts
   *   An array of parts to match.
   *
   *   Each rule has two parts (so far) for each component. One is the 'source'
   *   (which is most of the time the current value of the component based on
   *   the request) and one is the 'dest' which is the value of the component in
   *   case of a redirect. When matching a component, we sometime want to match
   *   more parts merged together. In this case, we can specify them in the
   *   $match_parts array, in the order of importance. For example, if there are
   *   two parts specified (let's say 'dest' and 'source'), then when getting
   *   the values of the component to match in the getComponentValues() method
   *   below, we will overwrite the value in the 'dest' part with the values in
   *   the 'source' part, but only if the value in the 'dest' part is 'any'.
   */
  public function matchComponent($entity, RedirectorComponentInterface $component, $match_parts = array('source')) {
    return $component->matchValue($this->getComponentValues($entity, $component, $match_parts));
  }

  /**
   * Returns an array with all the values of a rule that relates to a component.
   *
   * @param stdClass $entity
   *   The entity to which contains the rule definition.
   *
   * @param RedirectorComponentInterface $component
   *  A rule component.
   *
   * @param string $parts
   *  Which parts of the the rule to return for the component. Possible values
   *  so far: 'source', 'dest'.
   *  @see RedirectorRuleEntityController::matchComponent() for more
   *  explanations and how to use this.
   *
   *  @todo: this is an important method, we should do unit tests for it.
   */
  public function getComponentValues($entity, RedirectorComponentInterface $component, $parts = array('source')) {
    $columns = $component->getStorageColumns();
    $rule_values = array();
    foreach($columns as $column) {
      foreach ($parts as $part) {
        if (isset($entity->{$part . '_' . $column})) {
          // If we have not value yet for the column, then we just use the one
          // from the current part.
          if (!isset($rule_values[$column])) {
            $rule_values[$column] = $entity->{$part . '_' . $column};
            continue;
          }
          // If we are here, we check that if the current rule_values for the
          // column are set to '--any--', and if yes they are overwritten by
          // the current part.
          if (empty($rule_values[$column]['negate']) ) {
            if ((is_string($rule_values[$column]['value']) && $rule_values[$column]['value'] == '--any--') ||
                (is_array($rule_values[$column]['value']) && count($rule_values[$column]['value']) == 1 && !empty($rule_values[$column]['value']['--any--']))) {
              $rule_values[$column] = $entity->{$part . '_' . $column};
            }
          }
        }
      }
    }
    return $rule_values;
  }

  /**
   * Prepares the parameters for the redirect based on rule entity.
   *
   * @param stdClass $entity
   *   The entity to which contains the rule definition.
   *
   * @param string $url
   *   The original URL.
   *
   * @param array $options
   *   An array with the additional options for the URL.
   */
  public function prepareComponentRedirect($entity, RedirectorComponentInterface $component, &$url, &$options) {
    $component_values = $this->getComponentValues($entity, $component, array('dest'));
    $component->prepareRedirect($component_values, $url, $options);
  }

/* (non-PHPdoc)
 * @see EntityAPIControllerInterface::export()
 */public function export($entity, $prefix = '') {
    // TODO Auto-generated method stub
    }

/* (non-PHPdoc)
 * @see EntityAPIControllerInterface::import()
 */public function import($export) {
    // TODO Auto-generated method stub
    }

/* (non-PHPdoc)
 * @see EntityAPIControllerInterface::buildContent()
 */public function buildContent($entity, $view_mode = 'full', $langcode = NULL) {
    // TODO Auto-generated method stub
    }

/* (non-PHPdoc)
 * @see EntityAPIControllerInterface::view()
 */public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    // TODO Auto-generated method stub
    }
}
