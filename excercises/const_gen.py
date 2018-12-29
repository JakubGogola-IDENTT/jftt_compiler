import sys

class ConstantGenerator:

    def __init__(self, const):
        if const < 0:
            sys.stderr.write('You\'re an idiot!')
            exit()

        self.const = const
        self.instr_cost = {
            'INC': 1,
            'ADD': 5
        }
        self.output_code = None


    def gen_code(self):
        # Const is 0 or 1
        if self.const == 0:
            self.output_code = ['SUB B B', 'PUT B', 'HALT']
            self.write_to_file()
            return
        elif self.const == 1:
            self.output_code = ['SUB B B', 'INC B', 'PUT B', 'HALT']
            self.write_to_file()
            return

        # Const >= 2
        # Generate const using only INC 
        code_inc = []
        cost_inc = 0
        code_inc.append('SUB B B')

        for _ in range(0, self.const):
            code_inc.append('INC B')
            cost_inc += self.instr_cost['INC']
        
        code_inc.append('PUT B')
        code_inc.append('HALT')

        # Generate const using ADD and INC
        code_mixed = []
        cost_mixed = 0
        code_mixed.append('SUB B B')

        # How many '2' is in const
        # distr = int(self.const / 2)

        # Generate '2'
        code_mixed.append('INC B')
        code_mixed.append('INC B')
        cost_mixed += 2 * self.instr_cost['INC']
        tmp = 2

        while True:
            tmp *=  2
            if tmp <= self.const:
                code_mixed.append('ADD B B')
                cost_inc += self.instr_cost['ADD']

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





