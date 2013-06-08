/**
 * @module sio
 */
var sio = sio || {};

/**
 * @class SIOClient
 */
sio.Client = {

/**
 * @method getNCenter
 * @return A value converted from C/C++ "Poco::NotificationCenter*"
 */
getNCenter : function () {},

/**
 * @method getUri
 * @return A value converted from C/C++ "std::string"
 */
getUri : function () {},

/**
 * @method send
 * @param {std::string}
 */
send : function () {},

/**
 * @method fireEvent
 * @param {const char*}
 * @param {Ptr}
 */
fireEvent : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method disconnect
 */
disconnect : function () {},

/**
 * @method emit
 * @param {std::string}
 * @param {std::string}
 */
emit : function () {},

/**
 * @method connect
 * @return A value converted from C/C++ "SIOClient*"
 * @param {std::string}
 */
connect : function () {},

/**
 * @method SIOClient
 * @constructor
 * @param {std::string}
 * @param {std::string}
 * @param {SIOClientImpl*}
 */
SIOClient : function () {},

};
