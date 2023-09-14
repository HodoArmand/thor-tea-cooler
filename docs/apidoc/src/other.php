<?php

function notFound()
{
    /**
     * @api               {get} /notFound notFound
     * @apiName           notFound
     * @apiGroup          Other
     * 
     * 
     * @apiSuccessExample {json} Success-Response:
     *     HTTP/1.1 404 Not Found
     *      {
     *          "status": "Not found.",
     *          "msg": "The requested resource/route was not found.",
     *      }
     */
}

exit();
