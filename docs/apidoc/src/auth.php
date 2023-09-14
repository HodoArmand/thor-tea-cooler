<?php

function login()
{
    /**
     * @api               {post} /login login
     * @apiName           login
     * @apiGroup          Auth
     *
     * @apiHeader (loginApiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (loginApiHeader) {String} Accept application/json
     * 
     * @apiParamExample {json} Request-Example:
     * {
     *      "name": "required|minLength:3|maxLength:32",
     *      "password": "required|minLength:8|maxLength:32",
     * }
     *
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 201 CREATED
     *     {
     *          "status" : "ok",
     *          "msg"    : "e94nqfyzzetavgwdfv8zdd4pz2tor71y7wuv6l6huk11ipo13A",
     *     }
     *
     * @apiError          Unauthorized Wrong username or password.
     * @apiErrorExample   {json} Bad credentials Error-Response:
     *     HTTP/1.1 401 Unauthorized
     *     {
     *       "status": "Bad credentials.",
     *       "msg": "The provided login credentials don't match any of our records."
     *     }
     * @apiError          Bad_Request Invalid HTTP Request.
     * @apiErrorExample   {json} Bad Request Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Bad Request.",
     *       "msg": "Missing or incorrect request fields.",
     *       "fieldErrors": [
     *              "Missing 'name' request parameter."
     *       ],
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "he provided login credentials are correct, but there has been an error when saving them to our system."
     *     }
     */
}

function logout()
{
    /**
     * @api               {post} /logout logout
     * @apiName           logout
     * @apiGroup          Auth
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
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
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The provided logout credentials are correct, but there has been an error when saving the changes to our system."
     *     }
     */
}

function registerUser()
{
    /**
     * @api               {post} /registerUser registerUser
     * @apiName           registerUser
     * @apiGroup          Auth
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * @apiParamExample {json} Request-Example:
     * {
     *      "name": "required|minLength:3|maxLength:32 , must be unique Username",
     *      "password": "required|minLength:8|maxLength:32",
     *      "password_confirmed": "required|minLength:8|maxLength:32|same:password",
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
     *       "fieldErrors": [
     *              "Missing 'name' request parameter."
     *       ],
     *     }
     * @apiError          Name_taken User with given name already exists.
     * @apiErrorExample   {json} Name taken Error-Response:
     *     HTTP/1.1 400 Bad Request
     *     {
     *       "status": "Name taken.",
     *       "msg": "The provided name is already taken.",
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The provided credentials are correct, but there has been an error when saving them to our system."
     *     }
     */
}

function editUser()
{
    /**
     * @api               {put} /editUser editUser
     * @apiName           editUser
     * @apiGroup          Auth
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * @apiParamExample {json} Request-Example:
     * {
     *      "name": "required|minLength:3|maxLength:32 , must be unique Username",
     *      "password": "required|minLength:8|maxLength:32",
     *      "password_confirmed": "required|minLength:8|maxLength:32|same:password",
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
     *       "fieldErrors": [
     *              "Missing 'name' request parameter."
     *       ],
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The provided credentials are correct, but there has been an error when saving them to our system."
     *     }
     */
}

function deleteUser()
{
    /**
     * @api               {delete} /deleteUser deleteUser
     * @apiName           deleteUser
     * @apiGroup          Auth
     *
     * @apiHeader (apiHeader) {String} Content-Type application/x-www-form-urlencoded
     * @apiHeader (apiHeader) {String} Accept application/json
     * @apiHeader (apiHeader) {String} Authorization 'Bearer| apiKey' or 'Bearer apikey'
     * 
     * @apiParamExample {json} Request-Example:
     * {
     *      "password": "required|minLength:8|maxLength:32",
     *      "password_confirmed": "required|minLength:8|maxLength:32|same:password",
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
     *       "fieldErrors": [
     *              "password field must match password_confirmed."
     *       ],
     *     }
     * @apiError          (Error 500) Serverside_IO_error Serverside IO error. Most likely disk write IO error.
     * @apiErrorExample   {json} Serverside IO Error-Response:
     *     HTTP/1.1 500 Serverside IO error
     *     {
     *       "status": "Serverside IO error.",
     *       "msg": "The provided credentials are correct, but there has been an error when saving them to our system."
     *     }
     */
}

exit();
