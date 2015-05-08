<?php

/**
 * @file
 *  Controller class for the RedirectorTest Entity.
 */

class RedirectorRuleEntityController extends EntityAPIController {


  public function delete($ids) {
    db_delete($this->entityInfo['base table'])->condition('rid', $ids)->execute();
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
   *  And also a better explanation (maybe even some better namiing).
   */
  public function getComponentValues($entity, RedirectorComponentInterface $component, $parts = array('source')) {
    $rule_values = array();
    foreach ($parts as $part) {
      if (isset($entity->{$part}[$component->getId()])) {
        // If we have no value yet for the column, then we just use the one
        // from the current part.
        if (empty($rule_values)) {
          $rule_values = $entity->{$part}[$component->getId()];
          continue;
        }
        // If we are here, we check that if the current rule_values for the
        // column are set to '--any--', and if yes they are overwritten by
        // the current part. This make sense only if we do not have the 'negate'
        // setting.
        // @todo: maybe explain better this logic, because it is important and
        // it may be tricky to understand.
        if (empty($rule_values['negate']) ) {
          if ((is_string($rule_values['value']) && $rule_values['value'] == '--any--') ||
              (is_array($rule_values['value']) && count($rule_values['value']) == 1 && !empty($rule_values['value']['--any--']))) {
            $rule_values = $entity->{$part}[$component->getId()];
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
}
