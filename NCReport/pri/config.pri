#----------------------+---------------------------------------------------------------------
# VERSION
#----------------------+---------------------------------------------------------------------
APPVERSION = 2.15.0
WINDOWS_INSTALL_PATH = c:/ncreport

#----------------------+---------------------------------------------------------------------
# BUILD MODES          | DESCRIPTION
#----------------------+---------------------------------------------------------------------
# eval_ver             | Builds evaluation version
# debug_verbose        | Extended debug output mode
# nopixmap             | Use QImage instead of QPixmap
# translate_labels     | Qt translator integration for labels

BUILD_MODES += none

#-----------------------------+---------------------------------------------------------------
# DESIGNER TESTS              | DESCRIPTION
#-----------------------------+---------------------------------------------------------------
# db_mode_demo                | Load report from database demo
# custom_ds_demo              | Custom data source demo
# itemmodel_demo              | Item model data source demo
# reportsource_demo           | Custom report source demo
# testdatamenu                | Test data menu is enabled
# dragdrop_lf                 | Drag and drob label as field in ds. tree
# graphitemdialog_demo        | Demo of using custom graph item with private settings dialog
# stringlist_demo             | Generates string list datasources for a demo
# customfielddialog_demo      | Demo of using customized field settings dialog

DESIGNER_TESTS += none

#----------------------+---------------------------------------------------------------------
# DISABLED FEATURES    | DESCRIPTION
#----------------------+---------------------------------------------------------------------
# svg                  | No SVG input/output support
# sql                  | No SQL support
# preview_window       | Disable preview system
# email                | Disable email sending feature
# table_view           | Disable table view printing
# print_dialog         | Drag and drop label as field in Designer
# cursor               | Disables busy cursor on long processes

DISABLED_FEATURES += none

#----------------------+---------------------------------------------------------------------
# EXTENSIONS           | DESCRIPTION
#----------------------+---------------------------------------------------------------------
# highcharts           | HighCharts charting system integration (without official support!)
# markdown             | Markdown integration for Html text items
# webkit               | enable webkit module
# webengine            | enable webengine module (For Qt5.4<= only)

EXTENSIONS += none

include(config_defines.pri)


