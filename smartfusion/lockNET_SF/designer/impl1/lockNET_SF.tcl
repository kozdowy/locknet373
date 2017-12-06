# Created by Microsemi Libero Software 11.8.0.26
# Mon Dec 04 21:12:38 2017

# (OPEN DESIGN)

open_design "lockNET_SF.adb"

# set default back-annotation base-name
set_defvar "BA_NAME" "lockNET_SF_ba"
set_defvar "IDE_DESIGNERVIEW_NAME" {Impl1}
set_defvar "IDE_DESIGNERVIEW_COUNT" "1"
set_defvar "IDE_DESIGNERVIEW_REV0" {Impl1}
set_defvar "IDE_DESIGNERVIEW_REVNUM0" "1"
set_defvar "IDE_DESIGNERVIEW_ROOTDIR" {N:\PortableGit\locknet373\smartfusion\lockNET_SF\designer}
set_defvar "IDE_DESIGNERVIEW_LASTREV" "1"


# import of input files
import_source  \
-format "edif" -edif_flavor "GENERIC" -netlist_naming "VERILOG" {../../synthesis/lockNET_SF.edn} \
-format "pdc"  {..\..\component\work\lockNET_SF\lockNET_SF.pdc} -merge_physical "yes" -merge_timing "yes"
compile
report -type "status" {lockNET_SF_compile_report.txt}
report -type "pin" -listby "name" {lockNET_SF_report_pin_byname.txt}
report -type "pin" -listby "number" {lockNET_SF_report_pin_bynumber.txt}

save_design
