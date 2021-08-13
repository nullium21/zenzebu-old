workspace 'zenzebu-v2'
    architecture 'x64'

    configurations { 'debug', 'release' }

tgt_dir = '%{wks.location}/bin-%{cfg.architecture}/%{prj.name}-%{cfg.buildcfg}'
obj_dir = '%{wks.location}/obj-%{cfg.architecture}/%{prj.name}-%{cfg.buildcfg}'

include 'zenzebu'
include 'examples'
