# vitis -s setup.py hello1
# vitis --workspace ./workspace/

import vitis
import os
import sys
import shutil
from pathlib import Path

#app_name = sys.argv[1]
app_name = "tmc5240_test"
#app_name = "hello1"
print("app_name = ", app_name, "\n")

plat_name = "standalone_plat"
hw_xsa = "../implement/results/top.xsa"
cpu_name = "microblaze_0"
#cpu_name = "ps7_cortexa9_0"
#cpu_name = "psu_cortexa53_0"

#os.system('rm -rf workspace')
shutil.rmtree('workspace')

client = vitis.create_client()
client.set_workspace(path="workspace")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(
    name = plat_name,
    hw_design = hw_xsa,
    os = "standalone",
    cpu = cpu_name,
    domain_name = "standalone_domain_0",
    generate_dtb = False,
    advanced_options = advanced_options,
    compiler = "gcc"
)

#platform = client.get_component(name=plat_name)

status = platform.build()

# create empty application
comp = client.create_app_component(
    name=app_name,
    platform = "./workspace/standalone_plat/export/standalone_plat/standalone_plat.xpfm",
    domain = "standalone_domain_0",
    template = "empty_application"
)

# add source files as symbolic links
#os.system(f"ln -s ../../../src/{app_name}/test.c                    workspace/{app_name}/src/test.c")
#os.system(f"ln -s ../../../src/fpga.h                               workspace/{app_name}/src/fpga.h")
#os.system(f"ln -s ../../../src/tmc5240_spi.h                        workspace/{app_name}/src/tmc5240_spi.h")
#os.system(f"ln -s ../../../src/TMC5240/TMC5240_HW_Abstraction.h     workspace/{app_name}/src/TMC5240_HW_Abstraction.h")
target=Path("src/tmc5240_test/test.c").resolve();        link=Path("workspace/tmc5240_test/src/test.c");                      link.symlink_to(target)            
target=Path("src/fpga.h").resolve();                     link=Path("workspace/tmc5240_test/src/fpga.h");                      link.symlink_to(target)            
target=Path("src/tmc5240_spi.h").resolve();              link=Path("workspace/tmc5240_test/src/tmc5240_spi.h");               link.symlink_to(target)            
target=Path("src/TMC5240_HW_Abstraction.h").resolve();   link=Path("workspace/tmc5240_test/src/TMC5240_HW_Abstraction.h");    link.symlink_to(target)            

status = comp.clean()
status = platform.build()
comp.build()
vitis.dispose()

