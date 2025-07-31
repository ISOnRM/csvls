// Option.h
// An enum for parsing command line arguments
#ifndef OPTION_HPP
#define OPTION_HPP
#define AMOUNT_OF_OPTIONS 17
#define AMOUNT_OF_PRINTABLES 13
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
    Sort, // sort the list of options
	Help, // show help menu
	Recursive // enable recursion when walking dirs
};

#endif