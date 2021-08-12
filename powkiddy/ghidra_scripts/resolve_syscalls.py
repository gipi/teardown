# Resolve the syscall with the actual call inside the ucos-II
#@author Gianluca Pacchiella
#@category
#@keybinding 
#@menupath 
#@toolbar 
import zlib
import struct
import jarray
import re
import os

from ghidra.app.plugin.core.analysis import ConstantPropagationContextEvaluator
from ghidra.program.util import SymbolicPropogator
from ghidra.program.model.symbol import RefType, SourceType, Reference

#
# This is going to imitate ResolveX86orX64LinuxSyscallsScript.java
#



# Preliminary checks: you need the syscall definitions
PATH_LIBACTSEMI = "/opt/actsemi/libactsemi/actsemi.c"

if not os.path.exists(PATH_LIBACTSEMI):
    path = askFile(
        "Indicate the libactsemi/actsemi.c file to load syscall definition from (change PATH_LIBACTSEMI into the script to avoid this step)",
        "Load file"
    )

    PATH_LIBACTSEMI = path.path

# and the syscall calling convention
if not currentProgram.getFunctionManager().getCallingConvention("syscall"):
    popup("""sorry but you have to load the extension for the calling convention

    The file is named syscall.cspec.xml and should be in the directory containing this script""")

    raise Exception("syscall calling convention not imported")

def generate_syscalls_args_mapping(path):
    """Using the actsemi.c source file we generate the relations between
    argument value and syscalls' names"""
    mapping = {}

    c = re.compile("^SYSCALL\((?P<name>\w+),(?P<address>.*?)\)")

    with open(path, "r") as f:
        for line in f.readlines():
            if not line.startswith("SYSCALL"):
                continue

            m = c.match(line)

            if not m:
                raise ValueError("SYSCALL not recognized! check the regex please for '%s'!" % line)
            address = int(m.groupdict()['address'], 16)
            name    = m.groupdict()['name']

            # we are mapping the syscall argument to the name
            # since this makes simpler the next steps
            mapping[address] = name

    return mapping


def get_or_create_syscalls_memory_block_by_slot(slot):
    """The syscalls are grouped by the most significant part of the value in the
    register v1, so to avoid creating a huge memory block, we create 0x100 sized
    block of each group only where necessary."""
    block_name = "syscall_%d" % slot

    block = getMemoryBlock(block_name)

    if block:
        return block

    print "creating syscall space '%s'", block_name

    v = jarray.zeros(0x100, "b")
    return createMemoryBlock(block_name, toAddr("OTHER:0x00000000"), v, True)


def get_or_create_syscalls_memory_block_by_arg(arg):
    slot = (arg >> 16) & 0xff

    return get_or_create_syscalls_memory_block_by_slot(slot)


def check_contains_syscall(inst):
    return inst.getMnemonicString().startswith("syscall")


def resolve_argument(f, addr):
    register = currentProgram.getLanguage().getRegister("v1")

    startaddr = f.getEntryPoint()
    evaluator = ConstantPropagationContextEvaluator(True)
    symbolic = SymbolicPropogator(currentProgram)
    symbolic.flowConstants(startaddr, f.getBody(), evaluator, True, monitor);
    reg_value = symbolic.getRegisterValue(addr, register);

    return reg_value.getValue()


def get_all_syscall_invocations():
    functions = currentProgram.getFunctionManager().getFunctionsNoStubs(True)

    invocations = {}

    for function in functions:
        for instruction in currentProgram.getListing().getInstructions(function.getBody(), True):
            if check_contains_syscall(instruction):
                print "found syscall at %s@%s" % (function, instruction.getAddress())

                addresses = invocations.setdefault(function, [])
                addresses.append(instruction.getAddress())

    return invocations

def arg_to_address(arg):
    return arg & 0xffff

# HERE STARTS THE PLUGIN #####################################


maps_syscalls = generate_syscalls_args_mapping(PATH_LIBACTSEMI)
invocations = get_all_syscall_invocations()

print invocations

for function, addrcalls in invocations.iteritems():
    for addrcall in addrcalls:
        arg = resolve_argument(function, addrcall)
        syscall_name = maps_syscalls[arg]
        print "found v1: %08x -> %s" % (arg, syscall_name)

        syscall_space = get_or_create_syscalls_memory_block_by_arg(arg)
        syscall_addr = syscall_space.getStart().addressSpace.getAddress(arg_to_address(arg))
        syscall_function = currentProgram.getFunctionManager().getFunctionAt(syscall_addr);

        if not syscall_function:
            print "creating function for syscall '%s' at %08x" % (syscall_name, syscall_addr.getOffset())
            syscall_function = createFunction(syscall_addr, syscall_name)
            syscall_function.setNoReturn(True)
            syscall_function.setCallingConvention("syscall")

        if len(addrcalls) == 1 and function.name.startswith("FUN_"):
            new_name = '_%s' % syscall_name
            print "renaming '%s' -> '%s'" % (function.name, new_name)

            function.setName(new_name, SourceType.USER_DEFINED)

        reference = currentProgram.getReferenceManager().addMemoryReference(
            addrcall,
            syscall_addr,
            RefType.CALLOTHER_OVERRIDE_CALL,
            SourceType.USER_DEFINED,
            Reference.MNEMONIC,
        )
        # overriding references must be primary to be active
        currentProgram.getReferenceManager().setPrimary(reference, True);

