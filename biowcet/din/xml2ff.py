#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import xml.etree.ElementTree as ET
import re
folder = './build/'
path_xml = folder+'main.xml'
path_ff = folder+'main_empty.ff'

def set_ff(filename, iterations):
    with open(filename, 'r') as file, open(folder+'main_otawa.ff', 'w') as complete_file:
        content = file.read()

        lines = content.split('\n')

        lines_with_values = []

        for line in lines:
            if 'src/main.c' in line:
                if line.strip().startswith('loop'):
                    match = re.search(r'src/main\.c:(\d+)', line)
                    place = match.group(1)
                    for i in iterations:
                        if i['line'] == place:
                            if i['max'] == 'NOCOMP':
                                value = input("Insira as iteraçoes maximas do loop da linha {}: ".format(place))
                                line = line.replace('?', value)
                                lines_with_values.append(line)
                            else:
                                line = line.replace('?', i['max'])
                                lines_with_values.append(line)
                else:
                    lines_with_values.append(line)
            else:
                lines_with_values.append(line)
        
        for line in lines_with_values:
            complete_file.write(line + '\n')

        file.close()
        complete_file.close()

def extract_xml(filename):
    tree = ET.parse(filename)
    root = tree.getroot()

    # Dicionário para armazenar informações dos loops
    loops_info = []

    # Iterar pelas tags 'loop' dentro da tag 'function'
    for function in root.iter('function'):
        for loop in function.iter('loop'):
            if loop.get('line') is not None and loop.get('max') is not None and loop.get('min') is not None:
                loop_info = {
                    'line': loop.get('line'),
                    'max': loop.get('max'),
                    'min': loop.get('min')
                }
                loops_info.append(loop_info)

    return loops_info

iterations = extract_xml(path_xml)
location = set_ff(path_ff, iterations)
