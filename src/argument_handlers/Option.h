// Option.h
// An enum for parsing command line arguments
#ifndef OPTION_H
#define OPTION_H

enum class Option {
    ShowDev,        // st_dev
    ShowInode,      // st_ino
    ShowType,       // st_mode
    ShowPerms,      // st_mode
    ShowOwner,      // st_uid
    ShowGroup,      // st_gid
    ShowSize,       // st_size
    ShowBlocks,     // st_blocks
    ShowAccessTime, // st_atime
    ShowModTime,    // st_mtime
    ShowMetaModTime // st_ctime
};

#endif