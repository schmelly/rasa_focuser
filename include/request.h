#ifndef REQUEST_H
#define REQUEST_H

// struct Management_ApiVersions_RequestType
// {
// };

// struct Management_ApiVersions_RequestType
// {
// };

enum class AlpacaRequestType {
    // management request types
    management_apiversions,
    management_v1_description,
    management_v1_configureddevices,
    // common device request types
    common_connected,
    common_description,
    common_driverinfo,
    common_driverversion,
    common_interfaceversion,
    common_name,
    common_supportedactions,
    // focuser request types
    focuser_absolute,
    focuser_halt,
    focuser_ismoving,
    focuser_maxstep,
    focuser_maxincrement,
    focuser_move,
    focuser_position,
    focuser_stepsize,
    focuser_tempcomp,
    focuser_tempcompavailable,
    focuser_temperature
};

#endif // REQUEST_H