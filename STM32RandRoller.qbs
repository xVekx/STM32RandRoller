import qbs
import qbs.FileInfo
import "FileExtension.js" as FileExtension

//arm-none-eabi-gcc --print-multi-lib
//https://launchpad.net/~team-gcc-arm-embedded/+archive/ubuntu/ppa
//openocd -f interface/stlink-v2-1.cfg -f stm32f7.cfg
//openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg
//arm-none-eabi-objcopy -O ihex "${BuildArtifactFileBaseName}.elf" "${BuildArtifactFileBaseName}.hex
//picocom -b 115200 /dev/ttyUSB0
//arm-none-eabi-objdump -DS STM32RandRoller > r.txt


Product {
    Depends { name : "cpp" }
    type: ["application","size","hex"]
    consoleApplication: true
    cpp.positionIndependentCode: false
    name: "STM32RandRoller"

    cpp.linkerScripts: [
        "ldscripts/STM32F103C8Tx_FLASH.ld"
    ]

    cpp.commonCompilerFlags: [
        "-mthumb",
        "-mcpu=cortex-m3",
        "-fno-exceptions",
        "-fno-unwind-tables",
        "-fno-asynchronous-unwind-tables",
        "-ffunction-sections",
        "-fstrict-aliasing",
        "-fdata-sections",
        "-fno-builtin",
        ]

    cpp.linkerFlags: [
        "-mthumb",
        "-mcpu=cortex-m3",
        "-fno-exceptions",
        "-fno-unwind-tables",
        "-fno-asynchronous-unwind-tables",
        "-ffunction-sections",
        "-fstrict-aliasing",
        "-fdata-sections",
        "-fno-builtin",
        "-Xlinker",
        "--gc-sections",
        "-specs=nano.specs",
        //"-u _printf_float ",
        "-g",
        ]

    cpp.defines: [ "STM32F103xB","USE_HAL_DRIVER","BOARD_STM32F103C8T6"]

    cpp.cxxFlags: [ "-std=c++99" ]
    cpp.cFlags: [ "-std=gnu99" ]
    //cpp.cxxFlags: [ "-std=gnu++11" ]
    //cpp.cFlags: [ "-std=gnu11",]
    cpp.optimization: "small"
    cpp.warningLevel: "all"

    Group {
        files: [
              "ldscripts/STM32F103C8Tx_FLASH.ld"
        ]
        name: "ldscripts"
    }

    property string dir_path_stm32cube : "~/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.1/"

    cpp.includePaths : [

        "src",
        "usb",
        "stm32",

        dir_path_stm32cube + "Drivers/CMSIS/Include",
        dir_path_stm32cube + "Drivers/CMSIS/Device/ST/STM32F1xx/Include",

        dir_path_stm32cube + "Drivers/STM32F1xx_HAL_Driver/Inc/",
        dir_path_stm32cube + "Drivers/STM32F1xx_HAL_Driver/Inc/Legacy",

        dir_path_stm32cube + "Middlewares/ST/STM32_USB_Device_Library/Core/Inc",
        dir_path_stm32cube + "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc",

        ]

    Group {
        files: [
            "src/*.c",
            "src/*.h",
            "usb/*.h",
            "usb/*.c",
            "stm32/*.c",
            "stm32/*.h",
            "stm32/startup/startup_stm32f103xb.c",
        ]
        name: "APP"
    }

    Group {
        files: [

            dir_path_stm32cube + "Drivers/CMSIS/Include/core_cm3.h",
//            dir_path_stm32cube + "Drivers/CMSIS/Include/arm_math.h",
//            dir_path_stm32cube + "Drivers/CMSIS/Include/arm_const_structs.h"
            //dir_path_stm32cube + "Drivers/CMSIS/Include/core_cmFunc.h",
            //dir_path_stm32cube + "Drivers/CMSIS/Include/core_cmInstr.h",
            //dir_path_stm32cube + "Drivers/CMSIS/Include/core_cmSimd.h"

            ]
        name: "STM32Cube CORE"
    }

    Group {

        property string path : dir_path_stm32cube + "Drivers/CMSIS/Device/ST/STM32F1xx/"

        files: [
             path + "Include/stm32f1xx.h",
             path + "Include/system_stm32f1xx.h",
            ]
        name: "STM32Cube Startup"
    }

    Group {

        property string path : dir_path_stm32cube + "Drivers/CMSIS/DSP_Lib/Source/"

        files: [
             //path + "BasicMathFunctions/*",
             "~/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.1/Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/*"
            ]
        name: "STM32Cube CMSIS DSP_Lib"
    }

    Group {

        property string path : dir_path_stm32cube + "Drivers/STM32F1xx_HAL_Driver/"
        property string stm32fx : "stm32f1xx"


        files: [
            path + "Inc/"+stm32fx+"_hal.h",
            path + "Src/"+stm32fx+"_hal.c",
            path + "Inc/Legacy/stm32_hal_legacy.h",
            path + "Inc/"+stm32fx+"_hal_def.h",

            path + "Inc/"+stm32fx+"_hal_rcc.h",
            path + "Inc/"+stm32fx+"_hal_rcc_ex.h",
            path + "Src/"+stm32fx+"_hal_rcc.c",
            path + "Src/"+stm32fx+"_hal_rcc_ex.c",

            path + "Inc/"+stm32fx+"_hal_rtc.h",
            path + "Src/"+stm32fx+"_hal_rtc.c",
            path + "Inc/"+stm32fx+"_hal_rtc_ex.h",
            path + "Src/"+stm32fx+"_hal_rtc_ex.c",

            path + "Inc/"+stm32fx+"_hal_cortex.h",
            path + "Src/"+stm32fx+"_hal_cortex.c",

            path + "Inc/"+stm32fx+"_hal_tim.h",
            path + "Src/"+stm32fx+"_hal_tim.c",
            path + "Inc/"+stm32fx+"_hal_tim_ex.h",
            path + "Src/"+stm32fx+"_hal_tim_ex.c",

            path + "Inc/"+stm32fx+"_hal_pwr.h",
            path + "Src/"+stm32fx+"_hal_pwr.c",

            path + "Inc/"+stm32fx+"_hal_gpio.h",
            path + "Src/"+stm32fx+"_hal_gpio.c",
            path + "Inc/"+stm32fx+"_hal_gpio_ex.h",

            path + "Inc/"+stm32fx+"_hal_i2c.h",
            path + "Src/"+stm32fx+"_hal_i2c.c",

            path + "Inc/"+stm32fx+"_hal_spi.h",
            path + "Src/"+stm32fx+"_hal_spi.c",

            path + "Inc/"+stm32fx+"_hal_uart.h",
            path + "Src/"+stm32fx+"_hal_uart.c",

            path + "Inc/"+stm32fx+"_hal_usart.h",
            path + "Src/"+stm32fx+"_hal_usart.c",

            path + "Inc/"+stm32fx+"_hal_dma.h",
            path + "Src/"+stm32fx+"_hal_dma.c",
            path + "Inc/"+stm32fx+"_hal_dma_ex.h",
            //path + "Src/"+stm32fx+"_hal_dma_ex.c",

            path + "Inc/"+stm32fx+"_hal_pcd.h",
            path + "Src/"+stm32fx+"_hal_pcd.c",
            path + "Inc/"+stm32fx+"_hal_pcd_ex.h",
            path + "Src/"+stm32fx+"_hal_pcd_ex.c",

            path + "Inc/"+stm32fx+"_ll_usb.h",
            path + "Src/"+stm32fx+"_ll_usb.c",

            ]
        name: "STM32Cube HAL"
    }

    Group {

        property string path : dir_path_stm32cube + "Middlewares/ST/STM32_USB_Device_Library/"

        files: [
            path + "Core/Inc/usbd_core.h",
            path + "Core/Src/usbd_core.c",

            path + "Core/Inc/usbd_ctlreq.h",
            path + "Core/Src/usbd_ctlreq.c",

            path + "Core/Inc/usbd_def.h",
            path + "Core/Inc/usbd_ioreq.h",

            path + "Core/Src/usbd_ioreq.c",

            path + "Class/CDC/Inc/usbd_cdc.h",
            path + "Class/CDC/Src/usbd_cdc.c",

            ]

        name: "STM32Cube USB_DEVICE"
    }

    Rule {
        id: hex
        inputs: "application"
        Artifact {
            fileTags: "hex"
            filePath: "../../" + FileExtension.FileExtension(input.filePath, 1) + "/" + FileInfo.baseName(input.filePath) + ".hex"
        }
        prepare: {
            var args = ["-O", "ihex", input.filePath, output.filePath];
            var cmd = new Command("arm-none-eabi-objcopy", args);
            cmd.description = "converting to hex: "+FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
    }

    Rule {
         id: size
         inputs: "application"
         Artifact {
             fileTags: "size"
             filePath: "-"
         }
         prepare: {
             var args = [input.filePath];
             var cmd = new Command("arm-none-eabi-size", args);
             cmd.description = "File size: " + FileInfo.fileName(input.filePath);
             cmd.highlight = "linker";
             return cmd;
         }
    }

    Rule {
        id: disassembly
        inputs: "application"
        Artifact {
            fileTags: "disassembly"
            filePath: "../../" + FileExtension.FileExtension(input.filePath, 1) + "/" + FileInfo.baseName(input.filePath) + ".lst"
        }
        prepare: {
            var cmd = new Command("arm-none-eabi-objdump", [input.filePath, '-D','-S']);
            cmd.stdoutFilePath = output.filePath;
            cmd.description = "Disassembly listing for " + cmd.workingDirectory;
            cmd.highlight = "disassembler";
            cmd.silent = true;

            return cmd;
        }
    }
}
