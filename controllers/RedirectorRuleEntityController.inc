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
    $this->invoke($op, $entity);
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
   */
  public function matchComponent($entity, RedirectorComponentInterface $component) {
    return $component->matchValue($this->getComponentValues($entity, $component));
  }

  /**
   * Returns an array with all the values of a rule that relates to a component.
   *
   * @param stdClass $entity
   *   The entity to which contains the rule definition.
   *
   * @param RedirectorComponentInterface $component
   *  A rule component.
   */
  public function getComponentValues($entity, RedirectorComponentInterface $component) {
    $columns = $component->getStorageColumns();
    $rule_values = array();
    foreach($columns as $column) {
      if (isset($entity->{$column})) {
        $rule_values[$column] = $entity->{$column};
      }
    }
    return $rule_values;
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
