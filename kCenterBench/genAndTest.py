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
  timeout = 1
  genpath = "/home/miha/GraphGen/GraphGen/main"
  testpath = "/home/miha/kCenterBench/kCenterBench/main"
  #all_algs = ["greedy","greedyplus","gonzalez1c","gonzalezplus","plesnikdeg","hochbaumshmoys","CDS","CDSh"]
  all_algs = ["bf","reducebin"]
  #graph_types =["csrandom", "grid","scalefree"]
  graph_types =["scalefree"]
  densities = [1,0.7,0.5,0.3,0.1,0.01,0.001,0.0001]
  print("algorithm,graph_type,seed,n,density,score,execution_time")
  for graph_type in graph_types:
    for density in densities:
      algs = list(all_algs)
      n = 10
      while len(algs) > 0:
      #for i in range(100,101):
        #GEN
        f = tempfile.NamedTemporaryFile(mode='w',encoding='ascii')
        #print(f"generating test {n}...")
        result = gen([genpath,"-n",f"{n}","-f","pmed","-t",f"{graph_type}","-p",f"{density}","-s","42"],timeout*10,f)
        if result['status'] == "OK":
          pass
          #print(f"running test {n}...")
        elif result['status'] == "TIMEOUT": 
          print(f"timeout on generation")
          return
        else:
          print("ERROR in generation")
          print(f"stdout:\n{result['stdout']}\nstderr:\n{result['stderr']}\n")
          return
        f.flush()
        #TEST
        for alg in list(algs):
          result = test(testpath,f"{f.name}",alg,timeout)
          if result['status'] == "OK":
            #print(f"{alg:15} on n={n:5}: {result['solution']:6} {result['execution_time']:10} ms")
            print(f"{alg:{10}},{graph_type:{10}},42,{n},{density},{result['solution']},{result['execution_time']}")
          elif result['status'] == "TIMEOUT":
            #print(f"timeout({timeout}s)")
            print(f"{alg},{graph_type},42,{n},{density},-1,-1")
            algs.remove(alg)
          else:
            print("ERROR")
            print(f"stdout:\n{result['stdout']}\nstderr:\n{result['stderr']}\n")
            return
        n = n + 2
        print();
        f.close()
if __name__ == '__main__':
  main()
