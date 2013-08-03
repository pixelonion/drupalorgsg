<?php
/**
 * @file
 * Commerce payment forms for the ad hoc payment method.
 */

/**
 * Payment method callback: checkout form.
 */
function commerce_adhoc_payment_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array(
    'method' => array(
      '#type' => 'select',
      '#title' => t('Payment method'),
      '#options' => array(),
      '#required' => TRUE,
    ),
  );

  // Iterate over the available payment methods, adding them each as an option
  // to the payment method field.
  foreach (commerce_adhoc_payment_methods() as $method_name => $method) {
    $form['method']['#options'][$method_name] = $method['title'];

    // Iterate over any additional fields, adding each to the form with a
    // visible state of when the payment option is selected.
    foreach ($method['fields'] as $field_name => $field) {
      $field['#states'] = _commerce_adhoc_payment_method_state($method_name, $field['#required']);

      // Set the field to not required to avoid validation errors when the field
      // is not visible. Form validation will make sure any required fields for
      // the selected payment method are filled in and the states will allow the
      // field to appear as a normal required field.
      $field['#required'] = FALSE;

      // If a value has been set for the field then we need set it as the
      // default value.
      if (isset($pane_values[$field_name])) {
        $field['#default_value'] = $pane_values[$field_name];
      }

      // Add the field to the form.
      $form[$field_name] = $field;
    }
  }

  // Sort the payment methods and set the default value.
  asort($form['method']['#options']);
  $default_method = (isset($form['method']['#options']['cash']) ? 'cash' : '');
  $form['method']['#default_value'] = (isset($pane_values['method']) ? $pane_values['method'] : $default_method);

  $form['comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#default_value' => (isset($pane_value['comment']) ? $pane_value['comment'] : NULL),
    '#required' => TRUE,
  );

  // Allow other modules to alter this form before returning it.
  drupal_alter('commerce_adhoc_payment_method_form', $form);

  return $form;
}

/**
 * Payment method callback; validate callback for the payment submission form.
 */
function commerce_adhoc_payment_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  $payment_methods = commerce_adhoc_payment_methods();
  $payment_method = $payment_methods[$pane_values['method']];

  // Verify that any additional fields that may be required have been set.
  if (isset($payment_method['fields'])) {
    foreach ($payment_method['fields'] as $field_name => $field) {
      if ($field['#required'] && empty($pane_values[$field_name])) {
        form_set_error($field_name, t('@field field is required.', array('@field' => $field['#title'])));
      }
    }
  }
}

/**
 * Payment method callback: submit form submission.
 */
function commerce_adhoc_payment_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Create the payment transaction.
  $transaction = commerce_payment_transaction_new('adhoc', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->message = _commerce_adhoc_payment_transaction_message($pane_values);

  // Set the submitted values into the trasaction data for reference, except for
  // the comment.
  $transaction->data = $pane_values;
  unset($transaction->data['comment']);

  // Save the transaction.
  commerce_payment_transaction_save($transaction);
}

/**
 * Builds the visible form state for elements that should only be visible if a
 * certain payment method id selected.
 *
 * @param string $value
 *   The payment method that the element should be visible for.
 */
function _commerce_adhoc_payment_method_state($value, $required = FALSE) {
  $states = array(
    'visible' => array(
      ':input[name="payment_details[method]"]' => array(
        'value' => $value,
      ),
    ),
  );

  if ($required) {
    $states['required'] = array(
      ':input[name="payment_details[method]"]' => array(
        'value' => $value,
      ),
    );
  }

  return $states;
}

/**
 * Builds the message for the payment transaction that is created when an ad hoc
 * payment is saved.
 *
 * @param array $pane_values
 *   Form values that were submitted.
 *
 * @return string
 *   Message for the payment transaction.
 */
function _commerce_adhoc_payment_transaction_message($pane_values) {
  $payment_methods = commerce_adhoc_payment_methods();
  $payment_method = $payment_methods[$pane_values['method']];

  // First we want the payment method that the user selected.
  $message = array();
  $message[t('Payment method')] = $payment_method['title'];

  // Iterate over any additional fields specified for the selected payment
  // method.
  foreach ($payment_method['fields'] as $field_name => $field) {
    // Add the field and its value to the message. If the field has options
    // (e.g. a select field) then we need to get the human readable title from
    // the option that was selected.
    $message[$field['#title']] = (isset($field['#options'])
      ? $field['#options'][$pane_values[$field_name]]
      : $pane_values[$field_name]);
  }

  // Add the comment to the message.
  $message[t('Comment:')] = check_plain($pane_values['comment']);

  // Allow other modules to add their own lines to the message.
  drupal_alter('commerce_adhoc_payment_transaction_message', $message, $pan);

  return _commerce_adhoc_payment_format_transaction_message($message);
}

/**
 * Formats the payment transaction message.
 *
 * @param array $message
 *   Array of lines for the payment transaction message.
 *
 * @return string
 *   Formatted message for the payment transaction.
 */
function _commerce_adhoc_payment_format_transaction_message($message) {
  $formatted_message = '';
  foreach ($message as $label => $value) {
    $formatted_message .= "<b>$label:</b> $value<br />\n";
  }

  return $formatted_message;
}
