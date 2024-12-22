# ctemplate.py

import sys
import datetime
import os

class ctemplate:

    __fileName   = ' '
    __sourceName = ' '
    __headerName = ' '
    __cfgName    = ' '


    __time = datetime.datetime.now().strftime('%Y.%m.%d')
    __fileComments = {
                      '@file':" ",
                      '@author':'',
                      '@brief':' ',
                      '@date':__time,
                      '@version':'V01',
    }

    __CommentsOrder = ('@file','@author','@version','@date','@brief')

    __print_file_name = '#include \"lb_print.h\"'
    __assert_file_name = '#include \"lb_assert.h\"'
    def __init__(self,s,p):
        self.__fileName = s
        self.__sourceName = s + ".c"
        self.__headerName = s + '.h'
        self.__cfgName    = s + '_cfg.h'
        self.__path       = p

    def generateCommentsAtTop(self,name):
        self.__fileComments['@file'] = name

        comments = ("""/**
****************************************************************************************\n""")

        #find max length of string
        maxLen = 0
        for s in self.__CommentsOrder:
            if(len(s) > maxLen):
                maxLen = len(s)

        for k in self.__CommentsOrder:
            alignSpaceAmount = maxLen - len(k) + 4
            alignSpace = alignSpaceAmount * ' '
            if k == '@attention':
                 comments += ("""****************************************************************************************\n""")
            comments += ('* '+ k + alignSpace + self.__fileComments[k] + '\n')

        comments += ("""  ****************************************************************************************
  */\n\n""")

        return comments

    def generateCommentsInTheEnd(self):
        return ("/********************************************* END OF FILE *********************************************/\n")


    def generateCommentsInTheInclude(self):
        return ("""/*
 * INCLUDE FILES
 *****************************************************************************************
 */\n\n\n""")


    def generateCommentsInTheDefine(self):
        return ("""/*
 * DEFINES
 *****************************************************************************************
 */
 \n\n\n""")

    def generateCommentsInTheStructDefine(self):
        return ("""/*
 * STRUCT DEFINE
 *****************************************************************************************
 */\n\n\n""")

    def generateCommentsInTheLocalFunction(self):
        return ("""/*
 * LOCAL FUNCTION DECLARATION
 *****************************************************************************************
 */\n\n\n""")

    def generateCommentsInTheLocalVariable(self):
        return ("""/*
 * LOCAL VARIABLE DEFINITIONS
 *****************************************************************************************
 */\n\n\n""")


    def generateCommentsInTheLocalFunctionDefine(self):
        return ("""/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */\n\n\n""")

    def generateCommentsInTheGlobalFunctionDefine(self):
        return ("""/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */\n\n\n""")

    def createSource(self):
        print("start create file %s" %(self.__path + self.__fileName + '/' + self.__sourceName))
        fh = open(self.__path + self.__fileName + '/' + self.__sourceName,mode = 'w',encoding='utf-8')
        cm = self.generateCommentsAtTop(self.__sourceName)
        cm += self.generateCommentsInTheInclude()
        cm += ("#include \"%s\"\n" %self.__cfgName) 
        cm += ("#include \"%s\"\n" %self.__headerName) 
        """
        cm += ("\n\n\n%s\n" %self.__print_file_name)
        cm += ("%s\n" %self.__assert_file_name)
        """

        cm += ("\n"*3)
        cm += self.generateCommentsInTheDefine()
        cm += self.generateCommentsInTheStructDefine()

        cm += self.generateCommentsInTheLocalFunction()
        cm += self.generateCommentsInTheLocalVariable()
        cm += self.generateCommentsInTheLocalFunctionDefine()

        cm += self.generateCommentsInTheGlobalFunctionDefine()

        cm += ("\n"*10)
        cm += self.generateCommentsInTheEnd()
        fh.write(cm)
        fh.close()
        print("create file %s success!!!!" %(self.__path + self.__fileName + '/' + self.__sourceName))


    def generateCommentsInHfileInclude(self):
        return ("/* Includes ------------------------------------------------------------------*/\n")

    def generateCommentsInHfileExportTypes(self):
        return ("\n/* Exported types ------------------------------------------------------------*/\n")

    def generateCommentsInHfileExportFunction(self):
        return ("/* Exported functions --------------------------------------------------------*/\n")
    def createHeader(self):
        print("start create file %s" %(self.__path + self.__fileName + '/' + self.__headerName))
        fh = open(self.__path + self.__fileName + '/' + self.__headerName,mode = 'w',encoding='utf-8')
        cm = self.generateCommentsAtTop(self.__headerName)
        cm += "#ifndef __%s_H__\n" %self.__fileName.upper()
        cm += "#define __%s_H__\n" %self.__fileName.upper()
        cm += ("\n"*2)
        cm += self.generateCommentsInHfileInclude()
        cm += """#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif"""
        cm += self.generateCommentsInHfileExportTypes()
        cm += ("\n"*2)
        cm += self.generateCommentsInHfileExportFunction()
        cm += ("\n"*10)

        cm += """#ifdef __cplusplus
}
#endif\n"""
        cm += ("\n"*5)
        cm += "#endif\n"
        cm += self.generateCommentsInTheEnd()
        fh.write(cm)
        fh.close()
        print("create file %s success!!!!" %(self.__path + self.__fileName + '/' + self.__headerName))


    def createCfgHeader(self):
        print("start create file %s" %(self.__path + self.__fileName + '/' + self.__cfgName))
        fh = open(self.__path + self.__fileName + '/' + self.__cfgName,mode = 'w',encoding='utf-8')
        cm = self.generateCommentsAtTop(self.__cfgName)
        cm += "#ifndef __%s_CFG_H__\n" %self.__fileName.upper()
        cm += "#define __%s_CFG_H__\n" %self.__fileName.upper()
        cm += ("\n"*2)
        cm += self.generateCommentsInHfileInclude()
        cm += """#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif"""
        cm += self.generateCommentsInHfileExportTypes()
        cm += ("\n"*2)
        cm += self.generateCommentsInHfileExportFunction()
        cm += ("\n"*10)

        cm += """#ifdef __cplusplus
}
#endif\n"""
        cm += ("\n"*5)
        cm += "#endif\n"
        cm += self.generateCommentsInTheEnd()
        fh.write(cm)
        fh.close()
        print("create file %s success!!!!" %(self.__path + self.__fileName + '/' + self.__cfgName))


    def create_folder(self):
        folder = os.path.exists(self.__path + '/' + self.__fileName)
        is_have_folder = not folder
        print("is_have_folder = %d" %(is_have_folder))
        if is_have_folder:
            # os.makedirs 传入一个path路径，生成一个递归的文件夹；如果文件夹存在，就会报错,因此创建文件夹之前，需要使用os.path.exists(path)函数判断文件夹是否存在；
            os.makedirs(self.__path + '/' + self.__fileName)  # makedirs 创建文件时如果路径不存在会创建这个路径
            print('folder create success:', self.__path + self.__fileName)
        else:
            print('folder is already exist:', self.__path  + self.__fileName)

        return is_have_folder

    def createTemplatePairs(self):
        folder_create = self.create_folder()
        if folder_create:
            self.createSource()
            self.createHeader()
            self.createCfgHeader()
            


if __name__ == '__main__':
        if len(sys.argv) != 3:
            sys.stderr.write("please input corret parameter\r\n")
            sys.stderr.write("usage:python create_c_file_template file_name folder_path\r\n")
        else:
            s = sys.argv[1]
            create_path = sys.argv[2] #存在的问题：输入的路径最后要加上/ 应该增加判断
            ct = ctemplate(s,create_path)
            ct.createTemplatePairs()





""" oop module design
import sys
import datetime
import os


class cmodule:

    def __init__(self, module_name, module_path):
        self.module_name = module_name
        self.module_path = module_path

    def create_module(self):
        pass


"""
"""
os.makedirs 传入一个path路径,生成一个递归的文件夹,如果文件夹存在,就会报错,因此创建文件夹之前,需要使用os.path.exists(path)函数判断文件夹是否存在
"""
"""
makedirs 创建文件时如果路径不存在会创建这个路径
"""

"""
class cmodule_folder(cmodule):

    def __init__(self, module_name, module_path):
        super().__init__(module_name, module_path)

    def create_module(self):
        folder = os.path.exists(self.module_path + '/' + self.module_name)
        is_have_folder = not folder
        print("is_have_folder = %d" % (is_have_folder))
        if is_have_folder:
            os.makedirs(self.module_path + '/' + self.module_name)
            print('folder create success:',
                  self.module_path + self.module_name)


class cmodule_c_source_file(cmodule):

    def __init__(self, module_name, module_path):
        super().__init__(module_name, module_path)

    def create_module(self):
        print("create c_source_file" + self.module_name)


class cmodule_c_header_file(cmodule):

    def __init__(self, module_name, module_path):
        super().__init__(module_name, module_path)

    def create_module(self):
        print("create c_header_file" + self.module_name)


class cmodule_cfg_file(cmodule):

    def __init__(self, module_name, module_path):
        super().__init__(module_name, module_path)

    def create_module(self):
        print("create cfg_file" + self.module_name)


class cmodule_process:

    def __init__(self) -> None:
        pass

    def create_module(self, cmodule_type):
        cmodule_type.create_module()


if __name__ == "__main__":
    # print(sys.argv)
    module_process = cmodule_process()
    module_c = cmodule_c_source_file("lb_act", None)
    module_h = cmodule_c_header_file("lb_act", None)
    module_cfg = cmodule_cfg_file("lb_act", None)

    module_process.create_module(module_c)
    module_process.create_module(module_h)
    module_process.create_module(module_cfg)


"""