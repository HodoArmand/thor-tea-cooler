<?php

function getHardwareConfig()
{
    /**
     * @api               {get} /getHardwareConfig getHardwareConfig
     * @apiName           getHardwareConfig
     * @apiGroup          Configuration
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 200 OK
     *      {
     *          "status": "ok",
     *          "msg": "{\r\n  \"debugMode\": false,\r\n  \"relayIoPin1\": 25,\r\n  \"relayIoPin2\": 26,\r\n  \"oneWireIoPin\": 16,\r\n  \"temperatureSensorOffsetCelsius\": -2.20,\r\n  \"temperatureTargetDefault\": 50.00\r\n}"
     *      }
     *
     * @apiError          Unauthorized Bad API bearer token.
     * @apiErrorExample   {json} Unauthorized Error-Response:
     *     HTTP/1.1 403 Unauthorized
     *     {
     *       "status": "Unauthorized.",
     *       "msg": "Unauthorized to access this resource."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *       "fieldErrors": [
     *              "Bad Api header format."
     *       ],
     *     }
     */
}

function getNetworkConfig()
{
    /**
     * @api               {get} /getNetworkConfig getNetworkConfig
     * @apiName           getNetworkConfig
     * @apiGroup          Configuration
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 200 OK
     *      {
     *          "status": "ok",
     *          "msg": "{\r\n  \"debugMode\": false,\r\n  \"ssid\": \"ssid\",\r\n  \"password\": \"password\"\r\n}"
     *      }
     *
     * @apiError          Unauthorized Bad API bearer token.
     * @apiErrorExample   {json} Unauthorized Error-Response:
     *     HTTP/1.1 403 Unauthorized
     *     {
     *       "status": "Unauthorized.",
     *       "msg": "Unauthorized to access this resource."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *       "fieldErrors": [
     *              "Bad Api header format."
     *       ],
     *     }
     */
}

function getServerConfig()
{
    /**
     * @api               {get} /getServerConfig getServerConfig
     * @apiName           getServerConfig
     * @apiGroup          Configuration
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 200 OK
     *      {
     *          "status": "ok",
     *          "msg": "{\r\n  \"debugMode\": true,\r\n  \"port\": 80,\r\n  \"apiKeyLength\": 50,\r\n  \"apiThrottleIntervalMs\": 1000,\r\n  \"maxStoredUsers\": 20,\r\n  \"maxApiKeysPerUser\": 3,\r\n  \"maxApiKeysTotal\": 60,\r\n  \"selfHostMode\": false\r\n}"
     *      }
     *
     * @apiError          Unauthorized Bad API bearer token.
     * @apiErrorExample   {json} Unauthorized Error-Response:
     *     HTTP/1.1 403 Unauthorized
     *     {
     *       "status": "Unauthorized.",
     *       "msg": "Unauthorized to access this resource."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *       "fieldErrors": [
     *              "Bad Api header format."
     *       ],
     *     }
     */
}

function setHardwareConfig()
{
    /**
     * @api               {put} /setHardwareConfig setHardwareConfig
     * @apiName           setHardwareConfig
     * @apiGroup          Configuration
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * @apiParamExample {json} Request-Example:
     * {
     
     *    "debugMode": "required|bool",
     *    "relayIoPin1": "required|integer|gpioPin",
     *    "relayIoPin2": "required|integer|gpioPin",
     *    "oneWireIoPin": "required|integer",
     *    "temperatureSensorOffsetCelsius": "required|float",
     *    "temperatureTargetDefault": "required|float|between:20.00&75.00"
     * }
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *     {
     *          "status" : "ok",
     *          "msg"    : "ok",
     *     }
     *
     * @apiError          Unauthorized Bad API bearer token.
     * @apiErrorExample   {json} Unauthorized Error-Response:
     *     HTTP/1.1 403 Unauthorized
     *     {
     *       "status": "Unauthorized.",
     *       "msg": "Unauthorized to access this resource."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *        "fieldErrors": [
     *            "relayIoPin1 must be an integer number value.",
     *            "relayIoPin1: i is not a valid output pin.",
     *            "Missing x field, validation error or bad API header."
     *         ]
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error."
     *     }
     */
}

function setNetworkConfig()
{
    /**
     * @api               {put} /setNetworkConfig setNetworkConfig
     * @apiName           setNetworkConfig
     * @apiGroup          Configuration
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * @apiParamExample {json} Request-Example:
     * {
     *    "debugMode": "required|bool",
     *    "ssid": "required",
     *    "password": "required",
     * }
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *     {
     *          "status" : "ok",
     *          "msg"    : "ok",
     *     }
     *
     * @apiError          Unauthorized Bad API bearer token.
     * @apiErrorExample   {json} Unauthorized Error-Response:
     *     HTTP/1.1 403 Unauthorized
     *     {
     *       "status": "Unauthorized.",
     *       "msg": "Unauthorized to access this resource."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *        "fieldErrors": [
     *            "Missing x field, validation error or bad API header.",
     *         ]
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error."
     *     }
     */
}

function setServerConfig()
{
    /**
     * @api               {put} /setServerConfig setServerConfig
     * @apiName           setServerConfig
     * @apiGroup          Configuration
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * @apiParamExample {json} Request-Example:
     * {
     *    "debugMode": "required|bool",
     *    "port": "required|integer|between:0&65535",
     *    "apiKeyLength": "required|integer|between:10&254",
     *    "apiThrottleIntervalMs": "required|integer|min:0",
     *    "maxStoredUsers": "required|integer|min:1",
     *    "maxApiKeysPerUser": "required|integer|min:1",
     *    "maxApiKeysTotal": "required|integer|min:1",
     *    "selfHostMode": "required|bool";
     * }
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *     {
     *          "status" : "ok",
     *          "msg"    : "ok",
     *     }
     * @apiSuccessExample {json} Success-Response, API keylength changed:
     *     HTTP/1.1 201 CREATED
     *     {
     *          "status" : "ok, restarting",
     *          "msg"    : "Api key length has been changed, apikeys cleared. Device restarts in 10 seconds.",
     *     }
     *
     * @apiError          Unauthorized Bad API bearer token.
     * @apiErrorExample   {json} Unauthorized Error-Response:
     *     HTTP/1.1 403 Unauthorized
     *     {
     *       "status": "Unauthorized.",
     *       "msg": "Unauthorized to access this resource."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *        "fieldErrors": [
     *            "Missing x field, validation error or bad API header.",
     *         ]
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error."
     *     }
     */
}

exit();
