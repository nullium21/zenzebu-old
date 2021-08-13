workspace 'zenzebu-v2'
    architecture 'x64'

    configurations { 'debug', 'release' }

tgt_dir = '%{wks.location}/bin-%{cfg.architecture}/%{cfg.buildcfg}'
obj_dir = '%{wks.location}/obj-%{cfg.architecture}/%{cfg.buildcfg}'

group 'engine'
    include 'zenzebu'
group ''

include 'examples'
