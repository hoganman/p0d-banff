----------> uses
# use ROOT * 
#   use EXTERN * 
#   use MYSQL * 
#     use EXTERN * 
#   use GSL * 
#     use EXTERN * 
# use nd280Psyche * 
#   use psychePolicy v3r2 psyche
#   use psycheCore v3r22 psyche
#     use psychePolicy * psyche
#     use ROOT * 
#   use psycheEventModel v3r22 psyche
#     use psycheCore * psyche
#   use psycheUtils v3r22 psyche
#     use psycheEventModel * psyche
#   use psycheND280Utils v3r24 psyche
#     use psycheUtils * psyche
#   use psycheIO v3r18 psyche
#     use psycheCore * psyche
#     use psycheND280Utils * psyche
#   use psycheSelections v3r28 psyche
#     use psycheCore * psyche
#     use psycheEventModel * psyche
#     use psycheIO * psyche
#   use psycheSystematics v3r24 psyche
#     use psycheSelections * psyche
#   use psycheSteering v3r18 psyche
#     use psycheSystematics * psyche
#     use psycheSelections * psyche
#     use psycheIO * psyche
#     use psycheND280Utils * psyche
#     use psycheEventModel * psyche
#     use psycheCore * psyche
#
# Selection :
use CMT v1r20p20081118 (/physics/software)
use psychePolicy v3r2 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use EXTERN v3r7  (/physics/software/t2k/ND280/v11r31)
use GSL v1r15p0n00  (/physics/software/t2k/ND280/v11r31)
use MYSQL v5r051an01  (/physics/software/t2k/ND280/v11r31)
use ROOT v5r34p09n04  (/physics/software/t2k/ND280/v11r31)
use psycheCore v3r22 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheEventModel v3r22 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheUtils v3r22 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheND280Utils v3r24 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheIO v3r18 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheSelections v3r28 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheSystematics v3r24 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use psycheSteering v3r18 psyche (/physics/home/mhogan/software/t2k-software-clone/)
use nd280Psyche v3r26  (/physics/home/mhogan/software/t2k-software-clone)
----------> tags
CMTv1 (from CMTVERSION)
CMTr20 (from CMTVERSION)
CMTp20081118 (from CMTVERSION)
Linux (from uname) package CMT implies [Unix]
Linux-x86_64 (from CMTCONFIG)
software_config (from PROJECT) excludes [software_no_config]
software_root (from PROJECT) excludes [software_no_root]
software_cleanup (from PROJECT) excludes [software_no_cleanup]
software_prototypes (from PROJECT) excludes [software_no_prototypes]
software_without_installarea (from PROJECT) excludes [software_with_installarea]
software_with_version_directory (from PROJECT) excludes [software_without_version_directory]
software (from PROJECT)
ND280_config (from PROJECT) excludes [ND280_no_config]
ND280_root (from PROJECT) excludes [ND280_no_root]
ND280_cleanup (from PROJECT) excludes [ND280_no_cleanup]
ND280_prototypes (from PROJECT) excludes [ND280_no_prototypes]
ND280_without_installarea (from PROJECT) excludes [ND280_with_installarea]
ND280_with_version_directory (from PROJECT) excludes [ND280_without_version_directory]
x86_64 (from package CMT)
slc68 (from package CMT)
gcc447 (from package CMT)
Unix (from package CMT) excludes [WIN32 Win32]
----------> CMTPATH
# Add path /physics/home/mhogan/software/t2k-software-clone from initialization
# Add path /physics/software/t2k/ND280/v11r31 from initialization
