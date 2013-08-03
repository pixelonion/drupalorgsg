<?php
/**
 * @file
 * Commerce hook implementations for the Commerce Ad Hoc Payment module.
 */

/**
 * Implements hook_commerce_payment_method_info().
 */
function commerce_adhoc_payment_commerce_payment_method_info() {
  return array(
    'adhoc' => array(
      'base' => 'commerce_adhoc_payment',
      'title' => t('Ad hoc payment'),
      'short_title' => t('Ad hoc'),
      'display_title' => t('Ad hoc payment'),
      'description' => t('Add an ad hoc payment to an order.'),
      'file' => 'includes/commerce_adhoc_payment.forms.inc',
    ),
  );
}

/**
 * Implements hook_commerce_adhoc_payment_methods().
 *
 * Define the payment methods that are provided by default.
 */
function commerce_adhoc_payment_commerce_adhoc_payment_methods() {
  return array(
    'bank' => array(
      'title' => t('Bank transfer'),
    ),
    'cash' => array(
      'title' => t('Cash'),
    ),
    'check' => array(
      'title' => t('Check'),
      'fields' => array(
        'check_number' => array(
          '#type' => 'textfield',
          '#title' => t('Check #'),
          '#size' => 6,
          '#required' => TRUE,
        ),
      ),
    ),
    'credit' => array(
      'title' => t('Credit'),
    ),
    'credit_card' => array(
      'title' => t('Credit card'),
      'fields' => array(
        'credit_card_type' => array(
          '#type' => 'select',
          '#title' => t('Credit card type'),
          '#options' => _commerce_adhoc_payment_credit_card_types(),
          '#required' => TRUE,
        ),
      ),
    ),
    'debit' => array(
      'title' => t('Debit'),
    ),
    'ach' => array(
      'title' => t('ACH'),
    ),
    'online_payment' => array(
      'title' => t('Online payment'),
      'fields' => array(
        'online_payment_type' => array(
          '#type' => 'select',
          '#title' => t('Online payment type'),
          '#options' => _commerce_adhoc_payment_online_payment_types(),
          '#required' => TRUE,
        ),
      ),
    ),
    'services' => array(
      'title' => t('Services rendered'),
    ),
    'other' => array(
      'title' => t('Other'),
      'fields' => array(
        'other_payment_method' => array(
          '#type' => 'textfield',
          '#title' => t('Other'),
          '#required' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Returns the avialable credit card types.
 */
function _commerce_adhoc_payment_credit_card_types() {
  return array(
    'amex' => t('American Express'),
    'diners' => t('Diners club'),
    'discover' => t('Discover'),
    'jcb' => t('JCB'),
    'mastercard' => t('MasterCard'),
    'visa' => t('Visa'),
    'other' => t('Other'),
  );
}

/**
 * Returns the avialable online payment types.
 */
function _commerce_adhoc_payment_online_payment_types() {
  return array(
    '2checkout' => t('2Checkout'),
    'amazon' => t('Amazon'),
    'google' => t('Google Wallet'),
    'paypal' => t('PayPal'),
    'other' => t('Other'),
  );
}
