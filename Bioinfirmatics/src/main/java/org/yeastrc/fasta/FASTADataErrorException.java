package org.yeastrc.fasta;

/**
 * A data error has been found in the file being parsed.  
 * 
 * The standard Exception message holds the message about the data error.
 *
 */
public class FASTADataErrorException extends Exception {

	private static final long serialVersionUID = 1L;

	public FASTADataErrorException( String message ) {
		
		super(message);
	}
}
