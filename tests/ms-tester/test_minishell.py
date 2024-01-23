import json
import subprocess
import os

def run_command(command):
    result = subprocess.run(['../../minishell', '-c', command], capture_output=True)
    return result.stdout

def compare_json_files(file1, file2):
    try:
        with open(file1, 'r') as f1:
            data1 = json.load(f1)
    except (json.JSONDecodeError, ValueError):
        return False

    try:
        with open(file2, 'r') as f2:
            data2 = json.load(f2)
    except (json.JSONDecodeError, ValueError):
        return False

    return data1 == data2

def format_result(result):
    return 'OK' if result else '  KO'

def delete_files(*files):
    for file in files:
        if os.path.exists(file):
            os.remove(file)

def truncate_command(command, length=24):
    return (command if len(command) <= length else command[:length-4] + " ...")

def main():
    with open('test_cases.json', 'r') as file:
        test_cases = json.load(file)

    max_test_name_length = max(len(test['test_name']) for test in test_cases) + 1 # Additional padding for the first column

    print("{:<{}} {:<24} {:<8} {:<8}".format("TEST", max_test_name_length, "COMMAND", "TOKEN", "AST"))
    for test in test_cases:
        test_name = test['test_name'].replace(" ", "_")
        expected_tokens_file = os.path.join('results', f'{test_name}.tk.json')
        expected_ast_file = os.path.join('results', f'{test_name}.ast.json')

        run_command(test['command'])

        tokens_match = compare_json_files('./tokens.json', expected_tokens_file)
        ast_match = compare_json_files('./ast.json', expected_ast_file)

        truncated_command = truncate_command(test['command'])

        print("{:<{}} {:<24} {:<8} {:<8}".format(
            test['test_name'], max_test_name_length,
            truncated_command,
            format_result(tokens_match),
            format_result(ast_match)
        ))

        # delete_files('./tokens.json', './ast.json')

if __name__ == '__main__':
    main()
