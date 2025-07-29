// Option.h
// An enum for parsing command line arguments
#ifndef OPTION_H
#define OPTION_H

enum class Option {
    ShowType,        // st_mode
    ShowPerms,       // st_mode
    ShowNLinks,      // st_nlink
    ShowOwner,       // st_uid
    ShowGroup,       // st_gid
    ShowSize,        // st_size
    ShowAccessTime,  // st_atime
    ShowModTime,     // st_mtime
    ShowMetaModTime, // st_ctime
    Name,            // (имя записи)
    Canonical,       // (канонический путь имени)
    ShowInode,       // st_ino
    ShowBlocks,      // st_blocks
    ShowDev,         // st_dev

    // other options
    Sort // sort the list of options
};

#endif