/*
 * AUTOGENERATED BY make, DO NOT EDIT DIRECTLY
 */

char *
shmem_version(void)
{
  return "Super Happy Fun OpenSHMEM, Revision: 1334";
}

#ifdef HAVE_PSHMEM_SUPPORT
#pragma weak pshmem_version = shmem_version
#endif /* HAVE_PSHMEM_SUPPORT */
