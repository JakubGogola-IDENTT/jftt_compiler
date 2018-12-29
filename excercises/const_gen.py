import sys


class ConstantGenerator:

    def __init__(self, const):
        if const < 0:
            sys.stderr.write('You\'re an idiot!')
            exit()

        self.const = const
        self.instr_cost = {
            'INC': 1,
            'ADD': 5,
            'HALF': 1,
            'SUB': 5,
            'COPY': 5,
            'PUT': 100
        }
        self.output_code = None

    def gen_code(self):
        # Const is 0 or 1
        if self.const == 0:
            self.output_code = ['SUB B B', 'PUT B', 'HALT']
            print('Const is 0')
            self.write_to_file()
            return
        elif self.const == 1:
            self.output_code = ['SUB B B', 'INC B', 'PUT B', 'HALT']
            print('Const is 1')
            self.write_to_file()
            return

        # Const >= 2
        # Generate const using only INC 
        code_inc = []
        cost_inc = 0
        code_inc.append('SUB B B')
        cost_inc += self.instr_cost['SUB']

        for _ in range(0, self.const):
            code_inc.append('INC B')
            cost_inc += self.instr_cost['INC']
        
        code_inc.append('PUT B')
        code_inc.append('HALT')
        cost_inc += self.instr_cost['PUT']

        # Generate const using ADD and INC
        code_mixed = []
        cost_mixed = 0

        code_mixed.append('SUB B B')
        cost_mixed += self.instr_cost['SUB']

        # How many '2' is in const
        # distr = int(self.const / 2)

        # Generate '2' in register B
        code_mixed.append('INC B')
        code_mixed.append('INC B')
        cost_mixed += 2 * self.instr_cost['INC']

        # Value in register B
        tmp = 2

        while tmp + tmp <= self.const:
            tmp += tmp
            code_mixed.append('ADD B B')
            cost_mixed += self.instr_cost['ADD']

        # Half of value in register B
        code_mixed.append('SUB C C')
        code_mixed.append('COPY C B')
        code_mixed.append('HALF C')
        cost_mixed += self.instr_cost['SUB']
        cost_mixed += self.instr_cost['COPY']
        cost_mixed += self.instr_cost['HALF']

        half = tmp / 2

        while half != 2:
            if tmp + half <= self.const:
                tmp += half
                code_mixed.append('ADD B C')
                cost_mixed += self.instr_cost['ADD']

            if tmp == self.const:
                break
            
            code_mixed.append('HALF C')
            cost_mixed += self.instr_cost['HALF']
            half /= 2

        while tmp != self.const:
            code_mixed.append('INC B')
            cost_mixed += self.instr_cost['INC']
            tmp += 1

        code_mixed.append('PUT B')
        code_mixed.append('HALT')
        cost_mixed += self.instr_cost['PUT']

        # Check which solution is chepaer
        if cost_inc <= cost_mixed:
            self.output_code = code_inc
            print('Incrementing is faster')
        else:
            self.output_code = code_mixed
            print('Adding and incrementing is faster')

        print('******* COSTS *******')
        print('Incrementing: ', cost_inc)
        print('Adding and incrementing: ', cost_mixed)
        print('*********************')

        # END OF FUNCTION
        self.write_to_file()


    def write_to_file(self):
        file = open('const.mr', 'w')

        for item in self.output_code:
            file.write(item + '\n')
        
        file.close()


def main():
    const = int(input())
    gen = ConstantGenerator(const)
    gen.gen_code()

main()





