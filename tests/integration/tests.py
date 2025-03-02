import subprocess
import pytest

def run_calculator(input_expr, float_mode=False):
    """Запускает бинарный файл калькулятора и возвращает его вывод."""
    cmd = ['./build/app.exe'] if not float_mode else ['./build/app.exe', '--float']
    process = subprocess.run(cmd, input=input_expr, text=True, capture_output=True)
    return process.stdout.strip(), process.stderr.strip(), process.returncode

@pytest.mark.parametrize("expression, expected_output", [
    ("2 + 2", "4"),
    ("10 - 3", "7"),
    ("5 * 6", "30"),
    ("8 / 2", "4"),
    ("(2 + 3) * 4", "20"),
    ("10 / 3", "3"),
    ("-5 + 3", "-2"),
    ("(10 - (3 + 2)) * 2", "10"),
])
def test_integer_mode(expression, expected_output):
    output, err, code = run_calculator(expression)
    assert code == 0
    assert err == ""  # Ошибок не должно быть
    assert output == expected_output

@pytest.mark.parametrize("expression, expected_output", [
    ("2.2 + 2.3", "4.5000"),
    ("10.5 - 3", "7.5000"),
    ("5 * 6", "30.0000"),
    ("8.6 / 2", "4.3000"),
    ("10 / 3", "3.3333"),
    ("-5 + 3", "-2.0000"),
    ("(10 - (3 + 2)) * 2", "10.0000"),
])
def test_float_mode(expression, expected_output):
    output, err, code = run_calculator(expression, float_mode=True)
    assert code == 0
    assert err == ""  # Ошибок не должно быть
    assert output == expected_output

def test_invalid_expression():
    output, err, code = run_calculator("2 +")
    assert code != 0
    assert "Incomplete expression" in err

def test_division_by_zero():
    output, err, code = run_calculator("5 / 0")
    assert code != 0
    assert "Division by small number" in err

def test_large_numbers():
    output, err, code = run_calculator("2000000000 + 1")
    assert code != 0
    assert "is out of range" in err

def test_mismatched_parentheses():
    output, err, code = run_calculator("(2 + 3")
    assert code != 0
    assert "Incomplete expression" in err

def test_invalid_character():
    output, err, code = run_calculator("2 + a")
    assert code != 0
    assert "Invalid character" in err
