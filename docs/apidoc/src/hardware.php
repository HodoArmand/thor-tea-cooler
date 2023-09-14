<?php

function getHardwareState()
{
    /**
     * @api               {get} /getHardwareState getHardwareState
     * @apiName           getHardwareState
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 200 OK
     *      {
     *          "status": "ok",
     *          "msg": "{\r\n  \"relay1\": false,\r\n  \"relay2\": false,\r\n  \"temperature\": 22.80,\r\n  \"targetTemperature\": 50.00,\r\n  \"mode\": \"manual\"\r\n}"
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

function switchRelay()
{
    /**
     * @api               {post} /switchRelay switchRelay
     * @apiName           switchRelay
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiParamExample {json} Request-Example:
     * {
     *      "relay": "required|integer|inArray:1&2",
     * }
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "relay must be an integer number value."
     *       ],
     *     }
     */
}

function setRelays()
{
    /**
     * @api               {post} /setRelays setRelays
     * @apiName           setRelays
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiParamExample {json} Request-Example:
     * {
     *      "relay1": "required|bool",
     *      "relay2": "required|bool",
     * }
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function setModeManual()
{
    /**
     * @api               {post} /setModeManual setModeManual
     * @apiName           setModeManual
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function setModeAuto()
{
    /**
     * @api               {post} /setModeAuto setModeAuto
     * @apiName           setModeAuto
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function setTargetTemperature()
{
    /**
     * @api               {post} /setTargetTemperature setTargetTemperature
     * @apiName           setTargetTemperature
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     *
     * @apiParamExample {json} Request-Example:
     * {
     *      "targetTemperature": "required|float|between:20.00&75.00",
     * }
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function startAutoCooling()
{
    /**
     * @api               {post} /startAutoCooling startAutoCooling
     * @apiName           startAutoCooling
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function stopAutoCooling()
{
    /**
     * @api               {post} /stopAutoCooling stopAutoCooling
     * @apiName           stopAutoCooling
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "ok",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function restartMcu()
{
    /**
     * @api               {post} /restartMcu restartMcu
     * @apiName           restartMcu
     * @apiGroup          Hardware
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *      {
     *          "status": "ok",
     *          "msg": "Microcontroller is restarting in 10 seconds.",
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
     *              "Missing x field, validation error or bad API header."
     *       ],
     *     }
     */
}

function isTtc()
{
    /**
     * @api               {get} /isTtc isTtc
     * @apiName           isTtc
     * @apiGroup          Hardware
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 200 OK
     *      {
     *          "status": "yes",
     *          "msg": "Yes, TTC Device.",
     *      }
     */
}

exit();
