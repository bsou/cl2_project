
import java.io.*;

public class FileRename {
	
	public void rename() {
		
		String path = "data\\newunlabeledstatusfiles";
		final File folder = new File(path);
		File[] listOfFiles = folder.listFiles();
		
		int fileCount = 0;
		for (File file : listOfFiles) {
		    if (file.isFile()) {
		    	++fileCount;
		    	File newfile = new File(path + "\\" + "unlabuser-" + fileCount + ".txt");
		    	if(newfile.exists()) {
			    	System.out.println(newfile.getName() + " exists");
			    }
		    	
		    	boolean success = file.renameTo(newfile);
		    	if (!success) {
		    		System.out.println("File " + file.getName() + " was not successfully renamed");
		    	}
		    	
		    	System.out.println("Renamed file " + file.getName() + " to " + newfile.getName());
		    }
		}
		
		System.out.println("filecount: " + fileCount);
	}
	
	public static void main(String[] args) {
		FileRename fr = new FileRename();
		fr.rename();
	}

}
