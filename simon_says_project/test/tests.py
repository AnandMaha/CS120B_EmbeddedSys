# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).
tests = [
    
    {'description': 'Decrement PORTB from 7 to 4 by holding down pa1 for 100ms+100ms+1000ms+1000ms = 2200 ms',
    'steps': [ 
	{'inputs': [('PINA', ~0xFD)], 'time': 2200 }, 
    ],
    'expected': [('PORTB',0x04)],
    },
    {'description': 'Reset PORTB',
    'steps': [ 
	{'inputs': [('PINA', 0xFC)], 'time': 100 }, 
    ],
    'expected': [('PORTB',0x00)],
    },
    {'description': 'Increment PORTB from 0 to 6 for 100 ms + 5 * 1000 ms = 5100 ms',
    'steps': [ 
	{'inputs': [('PINA', 0xFE)], 'time': 5100 }, 
    ],
    'expected': [('PORTB',0x06)],
    },
    {'description': 'Rapidly decrement PORTB to 3 by pressing and releasing pa0 for (100 + 100) * 3 ms = 600 ms',
    'steps': [ 
	{'inputs': [('PINA', 0xFD)], 'time': 100 }, 
{'inputs': [('PINA', 0xFF)], 'time': 100 }, 
{'inputs': [('PINA', 0xFD)], 'time': 100 }, 
{'inputs': [('PINA', 0xFF)], 'time': 100 }, 
{'inputs': [('PINA', 0xFD)], 'time': 100 }, 
{'inputs': [('PINA', 0xFF)], 'time': 100 }, 
    ],
    'expected': [('PORTB',0x03)],
    },
   

]
# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['PORTB']

