#Parses ttydr_output.json into program.
#@author natv1337
#@category Labeling
#@keybinding
#@menupath
#@toolbar

import json
from java.io import File
from javax.swing import JFileChooser
from ghidra.util import Msg
from ghidra.program.model.symbol import SourceType
from ghidra.program.model.address import Address


def open_json_file():
    chooser = JFileChooser()
    chooser.setDialogTitle("Select your dumped output file.")
    chooser.setFileSelectionMode(JFileChooser.FILES_ONLY)

    if chooser.showOpenDialog(None) == JFileChooser.APPROVE_OPTION:
        return chooser.getSelectedFile()
    else:
        return None

def extract_name_from_declaration(declaration):
    str_before_parenthesis = declaration.split('(')[0].rstrip()
    str_before_parenthesis = str_before_parenthesis.split()[-1]
    return str_before_parenthesis.replace('&', '')

def create_label_at_address(address, name, namespace = "", object_name = ""):
    base_addr = currentProgram.getImageBase().getOffset()
    addr = toAddr(address + base_addr)

    label_name = name
    if object_name:
        label_name = "{}::{}".format(object_name, label_name)
    
    if namespace:
        label_name = "{}::{}".format(namespace, label_name)

    currentProgram.getSymbolTable().createLabel(addr, label_name, SourceType.USER_DEFINED)

file = open_json_file()
if file is None:
    Msg.showInfo(None, "Cancelled", "No file selected")
    exit()

try:
    with open(file.getAbsolutePath(), 'r') as f:
        json_data = json.load(f)

    Msg.showInfo(None, None, "Success", "Parsed JSON data.")

    for x in json_data['global_functions']:
        name = extract_name_from_declaration(x['declaration'])
        create_label_at_address(x['pointer'], name, x['namespace'])

    for x in json_data['global_properties']:
        name = extract_name_from_declaration(x['declaration'])
        create_label_at_address(x['pointer'], name, x['namespace'])

    for x in json_data['object_methods']:
        name = extract_name_from_declaration(x['declaration'])
        create_label_at_address(x['pointer'], name, x['namespace'], x['object'])

except Exception as e:
    Msg.showError(None, None, "Error", "An error has occured:\n{}".format(e))
