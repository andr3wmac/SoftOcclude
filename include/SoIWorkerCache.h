#pragma once
#ifndef __SOWORKERCACHE_H__
#define __SOWORKERCACHE_H__

#include "soIWorker.h"

// The main purpose of the cache is to collect worker jobs from the
// Api, to provide outside threading tools a way to catch the defined
// Work chunks
class SoIWorkerCache
{
public:

    virtual void AddWorkerJob(SoIWorker *) = 0;
};


#endif