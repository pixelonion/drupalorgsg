<?php
/**
 * @file
 * API documentation for hooks provided by the Commerce Adhoc Payment module.
 */

/**
 * Allows modules to alter the adhoc payment form.
 *
 * @param array $payment_method_form
 *   Payment method form.
 */
function hook_commerce_adhoc_payment_method_form_alter(&$payment_method_form) {
  $payment_method_form['comment']['#title'] = t('Comment title altered.');
}

/**
 * Allow modules to define payment methods available for selection when adding
 * an ad hoc payment.
 *
 * @return array
 *   Array of payment methods. The key should be the value of the of payment
 *   method (e.g. the machine name) and the value should be an array with the
 *   following keys:
 *   - title: Translated human readable title for the payment method.
 *   - fields (optional): An array of additional fields to be presented to the
 *     user when they select the payment method. The key should be the field
 *     should be prefixed with the module name to ensure uniqueness. The value
 *     should be a renderable FAPI form element array. #states will be set so
 *     that the field is only visible when the payment method is selected.
 *
 * @see commerce_adhoc_payment_methods()
 * @see commerce_adhoc_payment_commerce_adhoc_payment_methods()
 */
function hook_commerce_adhoc_payment_methods() {
  return array(
    'scholarship' => array(
      'title' => t('Scholarship'),
      'fields' => array(
        'commerce_adhoc_payment_scholarship_provider' => array(
          '#type' => 'textfield',
          '#title' => t('Provider'),
          '#description' => t('Enter the name of the organization that provided the scholarship.'),
        ),
      ),
    ),
  );
}

/**
 * Allow modules to alter the available payment methods for an ad hoc payment.
 *
 * @param array $payment_methods
 *   Array of availble payment methods.
 *
 * @see commerce_adhoc_payment_methods()
 * @see hook_commerce_adhoc_payment_methods()
 */
function hook_commerce_adhoc_payment_methods_alter(&$payment_methods) {
  // User may not select the "credit" payment method.
  unset($payment_methods['credit']);

  // Check payment method should ask for the name of the bank.
  $payment_methods['check']['fields']['commerce_adhoc_payment_check_bank'] = array(
    '#type' => 'bank',
    '#title' => t('Bank'),
  );
}

/**
 * Allow other modules to modify the message that will be included in the
 * commerce payment transaction.
 *
 * @param array $message
 *   Array of lines to include in the payment transaction message.
 * @param array $pane_values
 *   Array of values from the commerce payment transaction pane.
 *
 * @see _commerce_adhoc_payment_transaction_message()
 */
function hook_commerce_adhoc_payment_transaction_message_alter(&$message, $pane_values) {
  global $user;

  // Show the user that created the payment.
  $message[t('User')] = format_username($user);
}
