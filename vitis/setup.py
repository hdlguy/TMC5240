# vitis -s setup.py hello1
# vitis --workspace ./workspace/

import vitis
import sys
import shutil
from pathlib import Path

#app_name = sys.argv[1]
app_name = "tmc5240_test"
print("app_name = ", app_name, "\n")

plat_name = "standalone_plat"
hw_xsa = "../implement/results/top.xsa"
cpu_name = "microblaze_0"

shutil.rmtree('workspace', ignore_errors=True)

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

status = platform.build()

# create empty application
comp = client.create_app_component(
    name=app_name,
    platform = str(Path("./workspace/standalone_plat/export/standalone_plat/standalone_plat.xpfm").resolve()),
    domain = "standalone_domain_0",
    template = "empty_application"
)

# add source files as symbolic links
target=Path(f"src/{app_name}/test.c").resolve();         link=Path(f"workspace/{app_name}/src/test.c").resolve();                      link.symlink_to(target)            
target=Path(f"src/fpga.h").resolve();                    link=Path(f"workspace/{app_name}/src/fpga.h").resolve();                      link.symlink_to(target)            
target=Path(f"src/tmc5240_spi.h").resolve();             link=Path(f"workspace/{app_name}/src/tmc5240_spi.h").resolve();               link.symlink_to(target)            
target=Path(f"src/TMC5240_HW_Abstraction.h").resolve();  link=Path(f"workspace/{app_name}/src/TMC5240_HW_Abstraction.h").resolve();    link.symlink_to(target)            

status = comp.clean()
status = platform.build()
comp.build()
vitis.dispose()

