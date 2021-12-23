package embsystem2;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import embsystem2.embVO;

public class ConnectDB {
	// 싱글톤 패턴으로 사용 하기위 한 코드들
	Connection conn = null;
	PreparedStatement pstmt = null;
	String jdbc_driver = "com.mysql.cj.jdbc.Driver";
	String jdbc_url = "jdbc:mysql://192.168.231.187:3308/test?allowPublicKeyRetrieval=true&useUnicode=true&characterEncoding=utf8&useSSL=false&serverTimezone=UTC";
	//String jdbc_url = "jdbc:mysql://http://localhost/jspdb?allowPublicKeyRetrieval=true&useUnicode=true&characterEncoding=utf8&useSSL=false&serverTimezone=UTC";
	
	void connect() {
		try {
			Class.forName(jdbc_driver);
			conn = DriverManager.getConnection(jdbc_url, "testuser2", "raspberry");
			//conn = DriverManager.getConnection(jdbc_url, "jspbook", "passwd");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	void disconnect() {
		if (pstmt != null) {
			try {
				pstmt.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		if (conn != null) {
			try {
				conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}

		}
	}

	// 데이터베이스와 통신하기 위한 코드가 들어있는 메서드
	public ArrayList<embVO> infoscore() {
			connect();
			ArrayList<embVO> list1 = new ArrayList<embVO>();
			String sql = "select * from embsys";
			
			try {
				pstmt = conn.prepareStatement(sql);
				ResultSet rs = pstmt.executeQuery();
				while (rs.next()) {
					embVO a = new embVO();
					a.setMode(rs.getString("mode"));
					a.setName(rs.getString("name"));
					a.setScore(rs.getString("score"));
					list1.add(a);
				}
				rs.close();
			} catch (SQLException e) {
				e.printStackTrace();
			} finally {
				disconnect();
			}
			return list1;
			
	}

}