from pathlib import Path
import re

path = Path('gamemodes/includes/inventory/round.inc')
text = path.read_text('utf-8')
result = []
in_string = False
escape = False
buf = ''
pattern = re.compile(r'-?\d+\.\d+')

def format_num(num):
    try:
        f = float(num)
    except ValueError:
        return num
    return f'{round(f,1):.1f}'

for ch in text:
    if in_string:
        if escape:
            escape = False
            result.append(ch)
            continue
        if ch == '\\':
            escape = True
            result.append(ch)
            continue
        if ch == '"':
            in_string = False
            result.append(ch)
            continue
        result.append(ch)
    else:
        if ch == '"':
            if buf:
                result.append(pattern.sub(lambda m: format_num(m.group(0)), buf))
                buf = ''
            in_string = True
            result.append(ch)
            continue
        if ch.isdigit() or ch in '+-.':
            buf += ch
            continue
        else:
            if buf:
                result.append(pattern.sub(lambda m: format_num(m.group(0)), buf))
                buf = ''
            result.append(ch)

if buf:
    result.append(pattern.sub(lambda m: format_num(m.group(0)), buf))

new_text = ''.join(result)
backup = path.with_suffix(path.suffix + '.round1.bak')
backup.write_text(text, 'utf-8')
path.write_text(new_text, 'utf-8')
print('updated', path)
print('backup', backup)
