package embsystem2;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;
import org.json.JSONObject;

import embsystem2.ConnectDB;
import embsystem2.embVO;

/**
 * Servlet implementation class embservlet2
 */
@WebServlet("/embservlet2")
public class embservlet2 extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public embservlet2() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		doPost(request,response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		request.setCharacterEncoding("UTF-8");
		response.setContentType("UTF-8"); 
		
		PrintWriter out = response.getWriter();
		JSONObject jObject = new JSONObject();
		JSONArray jArray = new JSONArray();
		String jsonData = null;
		
		String cmdReq = "";
		cmdReq = request.getParameter("type");
				
		if ( cmdReq.equals("info")) {
			ConnectDB db = new ConnectDB();
			List<embVO> list1 = db.infoscore();
			
			try {
			for (embVO vo : list1) {
				JSONObject sObject = new JSONObject();
				sObject.put("mode", vo.getMode());
				sObject.put("name", vo.getName());
				sObject.put("score", vo.getScore());
				jArray.put(sObject);
			}
			jObject.put("Mdata", jArray);
			jsonData = jObject.toString();
			System.out.println(jObject.toString());
			
			out.print(jsonData);
			
			} catch (Exception e) {
				e.printStackTrace();
			}
			
		}
	}

}
