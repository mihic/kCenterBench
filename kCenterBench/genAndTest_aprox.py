import tempfile
import sys
import subprocess
import random

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
  timeout = 600 #s
  #soft_timeout = 60 #ms
  seed = 42
  outbuff = ""
  genpath = "/home/miha/GraphGen/GraphGen/main"
  testpath = "/home/miha/kCenterBench/kCenterBench/main"
  all_algs = ["CDSn4","greedy","greedyplus","greedyrnd","gonzalezplus","gonzalezrnd",
              "gonzalez1c","plesnikrnd","plesnikdeg","plesnikdeg+","bottleneck",
              "hochbaumshmoys","hochbaumshmoysbin","score","CDSPh","CDSP","CDS","CDSh"]
#  all_algs = ["plesnikdeg","plesnikrnd","gonzalezplus","greedy",
#              "gonzalez1c","bottleneck","greedyrnd","gonzalezrnd",
#              "hochbaumshmoysbin","score","CDS","CDSh"]
  
  #all_algs = ["bf","reducebin"]
  #all_algs = ["greedy", "gonzalez1c"]
  graph_types =["csrandom", "grid","scalefree"]
  #graph_types =["scalefree"]
  all_densities = [1,0.7,0.5,0.3,0.1,0.01,0.001,0.0001]

  print("algorithm,graph_type,seed,n,k,density,score,execution_time")
  for graph_type in graph_types:
    if graph_type=="grid":
      densities=[1]
    else:
      densities = all_densities
    for density in densities:
      algs = list(all_algs)
      n = 100
      seed = random.randint(0,100000)
      #GEN
      while (True):
        num_centers_list = set([3,5,10,n//2,n//3,n//4,n//5,n//6])
        too_long = False
        for k in num_centers_list:
          if (k>n//2+1):
            break;
          f = tempfile.NamedTemporaryFile(mode='w',encoding='ascii')
          #print(f"generating test {n}...")
          result = gen([genpath,"-n",f"{n}","-f","pmed","-t",f"{graph_type}","-p",f"{density}","-s",f"{seed}","-k",f"{k}"],timeout*10,f)
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
              #print(f"{alg:{10}},{graph_type:{10}},42,{n},{density},{result['solution']},{result['execution_time']}")

              #if int(result['execution_time'])>soft_timeout:
              #  too_long=True;

              outbuff += f"{alg},{graph_type},{seed},{n},{k},{density},{result['solution']},{result['execution_time']}\n"
            elif result['status'] == "TIMEOUT":
              #print(f"timeout({timeout}s)")
              #print(f"{alg},{graph_type},42,{n},{density},-1,-1")
              #algs.remove(alg)
              too_long=True;
              break; 
            else:
              print("ERROR")
              print(f"stdout:\n{result['stdout']}\nstderr:\n{result['stderr']}\n")
              return
          if (too_long):
            break;
        if (too_long):
          outbuff = ""
          break;
        else:
          sys.stdout.write(outbuff);
          outbuff = ""
          n = n + 100
          seed = random.randint(0,100000)
        f.close()
if __name__ == '__main__':
  main()
