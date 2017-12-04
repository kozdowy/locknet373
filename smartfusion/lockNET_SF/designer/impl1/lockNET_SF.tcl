# Created by Microsemi Libero Software 11.8.0.26
# Mon Dec 04 07:02:33 2017

# (OPEN DESIGN)

open_design "lockNET_SF.adb"

# set default back-annotation base-name
set_defvar "BA_NAME" "lockNET_SF_ba"
set_defvar "IDE_DESIGNERVIEW_NAME" {Impl1}
set_defvar "IDE_DESIGNERVIEW_COUNT" "1"
set_defvar "IDE_DESIGNERVIEW_REV0" {Impl1}
set_defvar "IDE_DESIGNERVIEW_REVNUM0" "1"
set_defvar "IDE_DESIGNERVIEW_ROOTDIR" {N:\Git\locknet373\smartfusion\lockNET_SF\designer}
set_defvar "IDE_DESIGNERVIEW_LASTREV" "1"


layout -timing_driven
report -type "status" {lockNET_SF_place_and_route_report.txt}
report -type "globalnet" {lockNET_SF_globalnet_report.txt}
report -type "globalusage" {lockNET_SF_globalusage_report.txt}
report -type "iobank" {lockNET_SF_iobank_report.txt}
report -type "pin" -listby "name" {lockNET_SF_report_pin_byname.txt}
report -type "pin" -listby "number" {lockNET_SF_report_pin_bynumber.txt}

save_design
