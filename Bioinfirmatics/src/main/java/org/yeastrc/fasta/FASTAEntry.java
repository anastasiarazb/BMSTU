/*
 *   Copyright 2015 Michael Riffle
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

package org.yeastrc.fasta;

import java.util.Set;

/**
 * Represents a header line + associated sequence in the FASTA file
*/
public final class FASTAEntry {

	public int hashCode() {
		return this.headers.hashCode() + this.sequence.hashCode();
	}

	public boolean equals( Object o ) {
		if( !( o instanceof FASTAEntry ) ) return false;

		if( !((FASTAEntry)o).getSequence().equals( this.getSequence() ) )
			return false;

		if( !((FASTAEntry)o).getHeaders().equals( this.getHeaders() ) )
			return false;

		return true;
	}

	/**
	 * Get an immutable FASTAEntry
	 * @param headers
	 * @param sequence
	 * @param headerLine
	 */
	public FASTAEntry( Set<FASTAHeader> headers, String sequence, String headerLine ) {
		this.headers = headers;
		this.sequence = sequence;
		this.headerLine = headerLine;
		this.headerLineNumber = -1; // set default
	}
	

	/**
	 * Get an immutable FASTAEntry
	 * @param headers
	 * @param sequence
	 * @param headerLine
	 */
	public FASTAEntry( Set<FASTAHeader> headers, String sequence, String headerLine, int headerLineNumber ) {
		this.headers = headers;
		this.sequence = sequence;
		this.headerLine = headerLine;
		this.headerLineNumber = headerLineNumber;
	}

	public Set<FASTAHeader> getHeaders() {
		return headers;
	}
	public String getSequence() {
		return sequence;
	}
	public String getHeaderLine() {
		return headerLine;
	}
	/**
	 * @return line number of header.  -1 is returned if not set
	 */
	public int getHeaderLineNumber() {
		return headerLineNumber;
	}

	private final Set<FASTAHeader> headers;
	private final String sequence;
	private final String headerLine;
	private final int headerLineNumber;


}
