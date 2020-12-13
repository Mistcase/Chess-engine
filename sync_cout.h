#pragma once
#ifndef _SYNC_COUT_INCLUED_
#define _SYNC_COUT_INCLUED_

#include <iostream>
#include <mutex>

enum SyncCout { IO_LOCK, IO_UNLOCK };
std::ostream& operator<<(std::ostream&, SyncCout);

#define sync_cout std::cout << IO_LOCK
#define sync_endl std::endl << IO_UNLOCK

#endif