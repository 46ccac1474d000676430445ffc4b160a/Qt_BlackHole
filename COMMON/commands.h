#ifndef COMMANDS_H
#define COMMANDS_H

enum Commands : int
{
    Authorization = 32,
    AuthorizationPassed,
    AuthorizationFailed,
    Authorized,


    Registration = 64,
    RegistrationPassed,
    RegistrationFailed,


    ClientSendPathName = 128,
    ServerAcceptedPath,
    ClientSendFile,
    ClientSendDir,

    PartOfFileAccepted,
    PartOfFileNotAccepted,

    ClientAskedDirList,
    DirExists,
    DirNotExists,

    ClientAskedFile,
    ClientAskedDir,

};

#endif // COMMANDS_H
