<?php

/**
 * @file
 *  Controller class for the RedirectorTest Entity.
 */

class RedirectorEntityController extends DrupalDefaultEntityController implements EntityAPIControllerInterface {

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
