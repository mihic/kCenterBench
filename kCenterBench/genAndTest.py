import tempfile
import sys
import subprocess

def test(prog_path,input_path,method,timeout):
  with subprocess.Popen([prog_path,"-i",input_path,"-m",method,"-f","pmed"],stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True) as process:
    try:
      process.wait(timeout=timeout)
      output = process.stdout.read()
      errput = process.stderr.read()
      if process.returncode == 0:
        solution,execution_time = output.strip().split(',')
        return {
            'status': 'OK',
            'solution': solution,
            'execution_time': execution_time}
      else:
        return {
            'status': 'FAIL',
            'returncode': process.returncode,
            'stdout': output,
            'stderr': errput}
    except subprocess.TimeoutExpired:
      process.kill()
      return {'status': 'TIMEOUT'}
      

def cat(input_path,timeout):
  with subprocess.Popen(["/bin/cat",input_path],stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True) as process:
    try:
      process.wait(timeout=timeout)
      output = process.stdout.read()
      errput = process.stderr.read()
      if process.returncode == 0:
        return {
            'status': 'OK',
            'solution': output}
      else:
        return {
            'status': 'FAIL',
            'returncode': process.returncode,
            'stdout': output,
            'stderr': errput}
    except subprocess.TimeoutExpired:
      process.kill()
      return {'status': 'TIMEOUT'}

def gen(parameter_list,timeout,f):
  with subprocess.Popen(parameter_list,stdout=f,stderr=subprocess.PIPE,universal_newlines=True,encoding='ascii') as process:
    try:
      process.wait(timeout=timeout)
      errput = process.stderr.read()
      if process.returncode == 0:
        return {
            'status': 'OK'}
      else:
        return {
            'status': 'FAIL',
            'returncode': process.returncode,
            'stderr': errput}
    except subprocess.TimeoutExpired:
      process.kill()
      return {'status': 'TIMEOUT'}

def main():
  timeout = 5000
  genpath = "/home/miha/GraphGen/GraphGen/main"
  testpath = "/home/miha/kCenterBench/kCenterBench/main"
  algs = ["greedy","CDSh"]
  for i in range(500,501):

    #GEN
    f = tempfile.NamedTemporaryFile(mode='w',encoding='ascii')
    print(f"generating test {i}...")
    result = gen([genpath,"-n",f"{i}","-f","pmed","-t","random","-p","0.01","-s","42"],timeout,f)
    if result['status'] == "OK":
      print(f"running test {i}...")
    elif result['status'] == "TIMEOUT":
      print(f"timeout on generation")
      return
    else:
      print("ERROR in generation")
      print(f"stdout:\n{result['stdout']}\nstderr:\n{result['stderr']}\n")
      return
    f.flush()

    #TEST
    for alg in algs:
      result = test(testpath,f"{f.name}",alg,timeout)
      if result['status'] == "OK":
        print(f"{alg:10} on n={i:5}: {result['solution']:4} {result['execution_time']:5} ms")
      elif result['status'] == "TIMEOUT":
        print(f"timeout({timeout}s)")
      else:
       print("ERROR")
       print(f"stdout:\n{result['stdout']}\nstderr:\n{result['stderr']}\n")
    f.close()
if __name__ == '__main__':
  main()
