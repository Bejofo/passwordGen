lines = open("public/target.txt").read().split("\n")
cleaned_lines = [x.split(":")[0] for x in lines if x != ""]
open("public/target.txt","w").write("\n".join(cleaned_lines))